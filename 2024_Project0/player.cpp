//========================================================================================
//
// プレイヤー
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "player.h"
#include "input.h"

#include "debugproc.h"
#include "camera.h"

#include "camera.h"

#include <stdio.h>
#include "fade.h"

#include "scene.h"
#include "sound.h"
#include "objectX.h"
#include "Xmanager.h"
#include <assert.h>
#include "model.h"
#include "Supporter.h"
#include "meshfield.h"
#include "audience.h"
#include <set>


//マクロ定義
#define MOVE_PLAYER (2.0f)
#define DASH_PLAYER (35.0f)
#define JUMP_PLAYER (20.0f)
//=============================================
//コンストラクタ
//=============================================
CPlayer::CPlayer(int nPriority):CObject(nPriority)
{
 	m_bLand = false;
	for (int nCnt = 0; nCnt < NUM_MODEL; nCnt++)
	{		
			m_apModel[nCnt] = NULL;
	
	}
	m_pEnemy = NULL;
	SetType(CObject::TYPE_PLAYER);

	m_nLife = 0;
	m_bShadow = true;
	m_bZ = true;
	m_pHitCol = NULL;
}
//=============================================
//デストラクタ
//=============================================
CPlayer::~CPlayer()
{
}

//=============================================
//初期化関数
//=============================================
HRESULT CPlayer::Init()
{
	m_nLand = 0;

	m_nScore = 0;
	m_pTarget = CAnimBillboard::Create(20.0f, 20.0f, 56, 5, 450, 30, true, GetPos(), "data\\TEXTURE\\lock2.jpg"); 
	//m_pTarget->m_bUI = true;
	m_nLife = PLAYERLIFE_MAX;
	m_pEnemy = DBG_NEW CEnemy*;
	*m_pEnemy = NULL;
	m_rotDest = VECTO3ZERO;
	m_pMotion = DBG_NEW CMotion;
	m_pMotion->SetModel(&m_apModel[0]);
	m_pMotion->Load("data\\TEXT\\motion_player2024_0.txt");
	m_bMotionLock = false;
	m_bKey = false;
	m_pOrbit = NULL;
	m_pColl = NULL;
	m_nDamage = 0;
	m_nRank = 0;
	m_nRankOld = 0;
	m_fPower = 0.0f;
	m_Size = 0.0f;
	m_attackpos = VECTO3ZERO;
	m_pRank = CObject2D::Create(D3DXVECTOR3(1000.0f, 200.0f, 0.0f), 125.0f, 125.0f, 6, "data\\TEXTURE\\rank.png");
	m_pScoreGage = CObject2D::Create(D3DXVECTOR3(950.0f, 270.0f, 0.0f), 10.0f, 125.0f,6,NULL,D3DXVECTOR2(0.0f,0.5f));

	m_pRank->SetDisp(false);
	for (int i = 0; i < 9; i++)
	{
		CAudience::Create();
	}

	m_pHitCol = CSphereCollision::Create(CSphereCollision::TYPE_PLAYER, 10.0f, 0, VECTO3ZERO, VECTO3ZERO, m_apModel[1]->GetMatrixAddress(), this);
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CPlayer::Uninit()
{
	DeletCollision();
	if (m_pMotion != NULL)
	{
		//m_pMotionUp->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}
	if (m_pOrbit != NULL)
	{
		m_pOrbit = NULL;
	}
	if (m_pEnemy != NULL)
	{
		delete m_pEnemy;
		m_pEnemy = NULL;
	}
	if (m_pHitCol != NULL)
	{
		delete m_pHitCol;
		m_pHitCol = NULL;
	}
	for (int nCnt = 0; nCnt < NUM_MODEL; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{
			m_apModel[nCnt]->Uninit();
			delete (m_apModel[nCnt]);
			m_apModel[nCnt] = NULL;
		}
	
	}
}
//=============================================
//更新関数
//=============================================
void CPlayer::Update()
{
	m_attackpos = D3DXVECTOR3{ 0.0f,0.0f,-40.0f };

	D3DXVec3TransformCoord(&m_attackpos, &m_attackpos, &m_mtx);
	m_posOld = GetPos();
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
	CInputMouse * pInputMouse = CManager::GetInstance()->GetInputMouse();
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
	for (int i = 0; i < CAudience::List.GetNum(); i++)
	{
		if (i <= m_nRank)
		{
			CAudience::List.Get(i)->SetActive(true);
		}
		else
		{
			CAudience::List.Get(i)->SetActive(false);
		}
	}
	
	if (pInputMouse->GetPress(CInputMouse::MOUSE_RIGTH) || pInputGamePad->GetPedalR(0))
	{
		Lockon();
	}
	else
	{
		*m_pEnemy = NULL;
	}
	if (*m_pEnemy == NULL)
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 50.0f;
		pCamera->SetRDest(pos);
		pCamera->SetLenght(+300.0f);
		m_pTarget->SetHeight(m_pTarget->GetHeight() + (100.0f - m_pTarget->GetHeight()) * 0.2f);
		m_pTarget->SetWidth(m_pTarget->GetWidth() + (100.0f - m_pTarget->GetWidth()) * 0.2f);
		m_pTarget->SetColor(m_pTarget->GetColor() + (D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) - m_pTarget->GetColor()) * 0.2f);
	}
	
	Action();
	StylishRank();

	SetPos(GetPos() + GetMove());
	D3DXVECTOR3 move = GetMove();
	move.x *= 0.9f;
	move.z *= 0.9f;
	SetMove(move);
	D3DXVECTOR3 animRot = D3DXVECTOR3(0.0f,  ((m_rotDest.y - GetRot().y)), 0.0f);
	if (animRot.y< -D3DX_PI)
	{
		animRot.y += D3DX_PI * 2;
	}
	else if (animRot.y > D3DX_PI)
	{
		animRot.y += -D3DX_PI * 2;
	}
	SetRot(GetRot() + animRot /6);
	CMeshfield * pMesh = CManager::GetInstance()->GetScene()->GetMeshfield();
	if (pMesh != NULL)
	{
		D3DXVECTOR3 move = GetMove();
		move.y -= GRAVITY;
		m_nLand--;
		if (pMesh->Collision(GetPosAddress()))
		{
			
				move.y = 0.0f;
				m_bLand = true;
				m_nLand = 3;
		}
		else
		{
			
			m_bLand = false;
		}
		SetMove(move);
	}
	D3DXVECTOR3 pos = GetPos();
	if (pos.x > 2000.0f)
	{
		pos.x = 2000.0f;
	}
	else if (pos.x < -3000.0f)
	{
		pos.x = -3000.0f;
	}
	if (pos.z > 450.0f)
	{
		pos.z = 450.0f;
	}
	else if (pos.z < -1000.0f)
	{
		pos.z = -1000.0f;
	}
	SetPos(pos);
}
//=============================================
//描画関数
//=============================================
void CPlayer::Draw()
{
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリクス

	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 pos = GetPos();


	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtx);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxTrans);

	//ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtx);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtx);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCnt = 0; nCnt < NUM_MODEL; nCnt++)
	{
		
		if (m_apModel[nCnt] != NULL)
		{
			m_apModel[nCnt]->Draw();
		}
	}
	//ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtx);

}
void CPlayer::StylishRank()
{
	m_nScore--;
	m_nRankOld = m_nRank;
	if (m_nScore < 0)
	{
		m_nScore = 0;
	}
	else if(m_nScore > SCORE_BORDER * 9)
	{
		m_nScore = SCORE_BORDER * 9;
	}
	m_nRank = m_nScore / SCORE_BORDER;
	if (m_nRank > 8)
	{
		m_nRank = 8;
	}
	if (m_nRankOld != m_nRank)
	{
		if (m_nRank > 0)
		{
			m_pRank->SetWidth(250.0f);
			m_pRank->SetHeight(250.0f);
			m_pRank->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	
		if (m_nRankOld < m_nRank)
		{
			m_nScore = SCORE_BORDER * m_nRank + SCORE_BORDER * 0.5f;
			
		}
	
	
	}
	if (m_nRank > 0)
	{
		m_pScoreGage->SetDisp(true);
		m_pScoreGage->SetWidth(125.0f * ((float) (m_nScore % SCORE_BORDER) / (float)SCORE_BORDER));
		m_pRank->SetWidth(m_pRank->GetWidth() + (125.0f - m_pRank->GetWidth())* 0.1f);
		m_pRank->SetHeight(m_pRank->GetHeight() + (125.0f - m_pRank->GetHeight())* 0.1f);
		m_pRank->SetCol(m_pRank->GetCol() + (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) - m_pRank->GetCol()) * 0.1f);
	
		m_pRank->SetDisp(true);
		m_pRank->SetTexMin(D3DXVECTOR2(1.0f - (1.0f / 8.0f) * (m_nRank), 0.0f));
		m_pRank->SetTexMax(D3DXVECTOR2(1.0f - (1.0f / 8.0f) * (m_nRank - 1), 1.0f));
	}
	else
	{
		m_pScoreGage->SetDisp(false);
		m_pRank->SetTexMin(D3DXVECTOR2(1.0f - (1.0f / 8.0f) * (1), 0.0f));
		m_pRank->SetTexMax(D3DXVECTOR2(1.0f - (1.0f / 8.0f) * (0), 1.0f));
		m_pRank->SetWidth(m_pRank->GetWidth() + (12.0f - m_pRank->GetWidth())* 0.1f);
		m_pRank->SetHeight(m_pRank->GetHeight() + (12.0f - m_pRank->GetHeight())* 0.1f);
		m_pRank->SetCol(m_pRank->GetCol() + (D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f) - m_pRank->GetCol()) * 0.1f);
	}
}
void CPlayer::Action()
{
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
	CInputMouse * pInputMouse = CManager::GetInstance()->GetInputMouse();

	m_pMotion->Update();
	AutoCollisionCreate();
	m_nMovecount++;
	if (m_nMovecount > 180 && !m_vActionLog.empty())
	{
		m_nMovecount = 0;
		m_vActionLog.erase(m_vActionLog.begin());
	}
	if (m_pMotion->GetType() < MOTION_COMBINATION1 && m_pMotion->GetType() != MOTION_DODGE)
	{
		m_bMotionLock = false;

	//	m_bKey = false;
		DeletCollision();
		if (m_pOrbit != NULL)
		{
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	if (!m_bMotionLock && m_pMotion->GetType() < MOTION_JUMP && m_nLand > 0)
	{
		CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
		D3DXVECTOR3 vec = pInputGamePad->GetStickL(0,0.1f);

		if (CManager::GetInstance()->GetDistance(vec) <= 0.9f || pInputKeyboard->GetPress(DIK_LSHIFT) || *m_pEnemy != NULL)
		{
			Walk();
		}
		else
		{
			Dash();
		}
		
	}

	if (!m_bMotionLock)
	{
		if (pInputMouse->GetTrigger(pInputMouse->MOUSE_LEFT) || pInputGamePad->GetTrigger(CInputGamePad::Button_Y,0))
		{
			if (m_pOrbit != NULL)
			{
				m_pOrbit->end();
				m_pOrbit = NULL;
			}
			m_nDamage = 0;
			m_fPower = 0.0f;
			m_Size = 0.0f;
			DeletCollision();
			m_bKey = true;
			CAudience::SetStateAll(CAudience::STATE_MANUALATTACK, 30);
			if (m_pMotion->GetType() < MOTION_COMBINATION1)
			{
				m_pMotion->SetType(MOTION_COMBINATION1);
				SetLog(MOTION_COMBINATION1);

			}
			else if (m_pMotion->GetType() < MOTION_MAX - 1)
			{
			
				m_pMotion->SetType(m_pMotion->GetType() + 1);
				SetLog((MOTION)m_pMotion->GetType());
			}
		}
	}
	if (!m_bMotionLock  && pInputGamePad->GetTrigger(CInputGamePad::Button_A, 0))
	{
		m_bKey = true;
		m_pMotion->SetType(MOTION_JUMP);
	}
	if (m_pMotion->GetType() != MOTION_DODGE  && pInputGamePad->GetTrigger(CInputGamePad::Button_B, 0))
	{
		m_bKey = true;
		if (m_pOrbit != NULL)
		{

			m_pOrbit->end();
			m_pOrbit = NULL;
		}
		SetMove(GetMove() + AnglesToVector(GetRot()) * -15.0f);
		m_pMotion->SetType(MOTION_DODGE);
	}
	Command();
	switch (m_pMotion->GetType())
	{
	case MOTION_JUMP:
		Jump();
		break;
	case MOTION_COMBINATION1:
		Attack1();
		break;
	case MOTION_COMBINATION2:
		Attack2();
		break;
	case MOTION_COMBINATION3:
		Attack3();
		break;
	case MOTION_COMBINATION4:
		Attack4();
		break;
	case MOTION_CHARGE:
		Charge();
		break;
	case MOTION_STREAK:
		Streak();
		break;
	case MOTION_HIGHROLLER:
		Highroller();
		break;
	case MOTION_SPLIT:
		Split();
		break;
	case MOTION_DAMAGE:
		Down();
		break;
	case MOTION_DODGE:
		Dodge();
		break;
	}

}
void  CPlayer::Command()
{
	if (*m_pEnemy != NULL && !m_bMotionLock)
	{
			CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
			D3DXVECTOR3 rot;
		
		CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
		D3DXVECTOR3 Vec = (*m_pEnemy)->GetPos() - GetPos();
		Vec.y = 0.0f;
		D3DXVECTOR3 Stick = VECTO3ZERO;
		Stick.x = pInputGamePad->GetStickL(0,0.5f).x;
		Stick.z = -pInputGamePad->GetStickL(0, 0.5f).y;
		if (Stick != VECTO3ZERO)
		{

			if (pCamera != NULL)
			{
				rot = pCamera->GetRot();

				D3DXMATRIX RotMtx;
				D3DXMatrixIdentity((&RotMtx));
				D3DXMatrixRotationY(&RotMtx, rot.y);
				D3DXVec3TransformCoord(&Stick, &Stick, &RotMtx);
				Stick *= -1;
				//向きを反映

			}

			if (GetAngleDifference(Vec, Stick) <= D3DX_PI * 0.5f &&  pInputGamePad->GetTrigger(CInputGamePad::Button_Y, 0) && m_nLand > 0)
			{
				Direction();
				m_pMotion->SetType(MOTION_CHARGE);
				SetLog((MOTION)m_pMotion->GetType());
			}
			if (GetAngleDifference(Vec, -Stick) <= D3DX_PI * 0.5f &&  pInputGamePad->GetTrigger(CInputGamePad::Button_Y, 0) && m_nLand > 0)
			{
				Direction();
				m_pMotion->SetType(MOTION_HIGHROLLER);
				SetLog((MOTION)m_pMotion->GetType());
			}
			if (GetAngleDifference(Vec, -Stick) <= D3DX_PI * 0.5f &&  pInputGamePad->GetTrigger(CInputGamePad::Button_Y, 0) && m_nLand <= 0)
			{
				Direction();
				m_pMotion->SetType(MOTION_SPLIT);
				SetLog((MOTION)m_pMotion->GetType());
			}
		}
	}
}
void  CPlayer::Charge()
{
	if (*m_pEnemy != NULL )
	{
		D3DXVECTOR3 Vec = (*m_pEnemy)->GetPos() - GetPos();
		Vec.y = 0.0f;
		m_bMotionLock = true;
		if (GetDistance(Vec) < 300.0f)
		{
			m_bKey = true;
			m_pMotion->SetType(MOTION_STREAK);
		}
		D3DXVec3Normalize(&Vec, &Vec);
		SetMove(Vec * 15.0f);
		Mirage();
	}
	else
	{
		m_pMotion->SetType(MOTION_STREAK);
	}
}
void CPlayer::Walk()
{
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
	D3DXVECTOR3 vec = VECTO3ZERO;
	D3DXVECTOR3 rot = VECTO3ZERO;
	if (pInputKeyboard->GetPress(DIK_W) || pInputKeyboard->GetPress(DIK_A)|| pInputKeyboard->GetPress(DIK_S)|| pInputKeyboard->GetPress(DIK_D))
	{
		if (pInputKeyboard->GetPress(DIK_W))
		{
			vec.z += 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_A))
		{
			vec.x -= 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_S))
		{
			vec.z -= 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_D))
		{
			vec.x += 1.0f;
		}
		
	}
	else
	{
		vec.x = pInputGamePad->GetStickL(0, 0.01f).x;
		vec.z = -pInputGamePad->GetStickL(0, 0.01f).y;
	}
	if (vec != VECTO3ZERO)
	{
		D3DXVec3Normalize(&vec, &vec);
		if (pCamera != NULL)
		{
			rot = pCamera->GetRot();

			D3DXMATRIX RotMtx;
			D3DXMatrixIdentity((&RotMtx));
			D3DXMatrixRotationY(&RotMtx, rot.y);
			D3DXVec3TransformCoord(&vec, &vec, &RotMtx);
			vec *= -1;
			//向きを反映
		
		}
		if (m_pMotion->GetKey() != 3 && m_pMotion->GetKey() != 7)
		{
			SetMove(GetMove() + vec*WALK_SPEED);
		}
		else
		{
			//SetMove(GetMove() + vec*WALK_SPEED * 0.25f);
		}
		m_rotDest.y = atan2f(-vec.x, -vec.z);
		
		m_pMotion->SetType(MOTION_WALK);
	}
	else
	{
		m_pMotion->SetType(MOTION_NEUTRAL);
	}
}
void CPlayer::Dash()
{
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
	D3DXVECTOR3 vec = VECTO3ZERO;
	D3DXVECTOR3 rot = VECTO3ZERO;
	if (pInputKeyboard->GetPress(DIK_W) || pInputKeyboard->GetPress(DIK_A) || pInputKeyboard->GetPress(DIK_S) || pInputKeyboard->GetPress(DIK_D))
	{
		if (pInputKeyboard->GetPress(DIK_W))
		{
			vec.z += 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_A))
		{
			vec.x -= 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_S))
		{
			vec.z -= 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_D))
		{
			vec.x += 1.0f;
		}

	}
	else
	{
		vec.x = pInputGamePad->GetStickL(0, 0.01f).x;
		vec.z = -pInputGamePad->GetStickL(0, 0.01f).y;
	}
	if (vec != VECTO3ZERO)
	{
		D3DXVec3Normalize(&vec, &vec);
		if (pCamera != NULL)
		{
			rot = pCamera->GetRot();

			D3DXMATRIX RotMtx;
			D3DXMatrixIdentity((&RotMtx));
			D3DXMatrixRotationY(&RotMtx, rot.y);
			D3DXVec3TransformCoord(&vec, &vec, &RotMtx);
			vec *= -1;
			//向きを反映

		}
		if (m_pMotion->GetKey() != 3 && m_pMotion->GetKey() != 7)
		{
			SetMove(GetMove() + vec*DASH_SPEED);
		}
		else
		{
			//SetMove(GetMove() + vec*WALK_SPEED * 0.25f);
		}
		m_rotDest.y = atan2f(-vec.x, -vec.z);

		m_pMotion->SetType(MOTION_DASH);
	}
	else
	{
		m_pMotion->SetType(MOTION_NEUTRAL);
	}
}
void CPlayer::Jump()
{
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
	D3DXVECTOR3 vec = VECTO3ZERO;
	D3DXVECTOR3 rot = VECTO3ZERO;
	if (pInputKeyboard->GetPress(DIK_W) || pInputKeyboard->GetPress(DIK_A) || pInputKeyboard->GetPress(DIK_S) || pInputKeyboard->GetPress(DIK_D))
	{
		if (pInputKeyboard->GetPress(DIK_W))
		{
			vec.z += 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_A))
		{
			vec.x -= 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_S))
		{
			vec.z -= 1.0f;
		}
		if (pInputKeyboard->GetPress(DIK_D))
		{
			vec.x += 1.0f;
		}

	}
	else
	{
		vec.x = pInputGamePad->GetStickL(0, 0.01f).x;
		vec.z = -pInputGamePad->GetStickL(0, 0.01f).y;
	}
	if (vec != VECTO3ZERO)
	{
		D3DXVec3Normalize(&vec, &vec);
		if (pCamera != NULL)
		{
			rot = pCamera->GetRot();

			D3DXMATRIX RotMtx;
			D3DXMatrixIdentity((&RotMtx));
			D3DXMatrixRotationY(&RotMtx, rot.y);
			D3DXVec3TransformCoord(&vec, &vec, &RotMtx);
			vec *= -1;
			//向きを反映

		}
	
			SetMove(GetMove() + vec*DASH_SPEED * 0.5f);
	
		m_rotDest.y = atan2f(-vec.x, -vec.z);

	}
	if (m_bKey && m_pMotion->GetKey() == 2)
	{
		m_bKey = false;
		
			SetMove(D3DXVECTOR3(GetMove().x, +10.0f, GetMove().z));
		
		
	}
	if (m_pMotion->GetKey() == 2 && m_bLand)
	{
		m_pMotion->SetType(MOTION_NEUTRAL);
	}
}
void CPlayer::Attack1()
{
	m_nDamage = 5;
	m_fPower = 7.0f;
	m_Size = 2.0f;
	Direction();
	if (m_pMotion->GetKey() == 2)
	{
		if (m_pOrbit == NULL)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SLASHSWING);
			m_pOrbit = COrbit::Create(30, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -54.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress(), "data\\TEXTURE\\OrbitBrade.png");
		}
		
		
	}
	if (m_pMotion->GetKey() >= 6)
	{
		if (m_pOrbit != NULL)
		{
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	else
	{
		Mirage();
	}
	if (m_pMotion->GetKey() <= 3)
	{
		m_bMotionLock = true;
	}
	else
	{
		m_bMotionLock = false;
	}
	if (m_bKey)
	{
		m_bKey = false;
		SetMove(GetMove() + AnglesToVector(GetRot()) * -7.0f);
		if (m_nLand <= 0)
		{
			SetMove(D3DXVECTOR3(GetMove().x, +5.0f, GetMove().z));
			
		}
	}
	
}
void CPlayer::Attack2()
{
	m_nDamage = 8;
	m_fPower = 5.0f;
	m_Size = 2.0f;
	Direction();
	if (m_pMotion->GetKey() == 2)
	{
		if (m_pOrbit == NULL)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SLASHSWING);
			m_pOrbit = COrbit::Create(30, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -44.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress(), "data\\TEXTURE\\OrbitBrade.png");
		}
	}
	if (m_pMotion->GetKey() >= 5)
	{
		if (m_pOrbit != NULL)
		{
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	else
	{
		Mirage();
	}
	if (m_pMotion->GetKey() <= 2)
	{
		m_bMotionLock = true;
	}
	else
	{
		m_bMotionLock = false;
	}
	if (m_bKey)
	{
		m_bKey = false;
		SetMove(GetMove() + AnglesToVector(GetRot()) * -7.0f);
		if (m_nLand <= 0)
		{
			SetMove(D3DXVECTOR3(GetMove().x, +5.0f, GetMove().z));
			
		}
	}
}
void CPlayer::Attack3()
{
	m_nDamage = 3;
	m_fPower = 10.0f;
	m_Size = 1.0f;
	Direction();
	if (m_pMotion->GetKey() == 1)
	{
		if (m_pOrbit == NULL)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SLASHSWING);
			m_pOrbit = COrbit::Create(30, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -44.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress(), "data\\TEXTURE\\OrbitBrade.png");
		}
	}
	if (m_pMotion->GetKey() >= 4)
	{
		if (m_pOrbit != NULL)
		{
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	else
	{
		Mirage();
	}
	if (m_pMotion->GetKey() <= 3)
	{
		m_bMotionLock = true;
	}
	else
	{
		m_bMotionLock = false;
	}
	if (m_bKey)
	{
		m_bKey = false;
		SetMove(GetMove() + AnglesToVector(GetRot()) * -10.0f);
		if (m_nLand <= 0)
		{
			SetMove(D3DXVECTOR3(GetMove().x, +5.0f, GetMove().z));
			
		}
	}
}
void CPlayer::Attack4()
{
	m_nDamage = 15;
	m_fPower = 20.0f;
	m_Size = 2.0f;
	if (m_pMotion->GetKey() == 2)
	{
		if (m_pOrbit == NULL)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SLASHSWING);
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_DESTRUCT);
			m_pOrbit = COrbit::Create(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -60.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress(), "data\\TEXTURE\\OrbitBrade.png");
		}
	}
	 if (m_pMotion->GetKey() >= 4)
	{
		if (m_pOrbit != NULL)
		{
		
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	 else
	 {
		 Mirage();
	 }
	if (m_pMotion->GetKey() <= 4)
	{
		m_bMotionLock = true;
	}
	else
	{
		m_bMotionLock = false;
	}
	if (m_bKey)
	{
		m_bKey = false;
		SetMove(GetMove() + AnglesToVector(GetRot()) * -4.0f);

	}
}

void CPlayer::Streak()
{
	m_nDamage = 10;
	m_fPower = 10.0f;
	m_Size = 2.0f;
	if (m_pMotion->GetKey() == 1)
	{
		if (m_pOrbit == NULL)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SLASHSWING);
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_DESTRUCT);
			m_pOrbit = COrbit::Create(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -100.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress(), "data\\TEXTURE\\OrbitBrade.png");
		}
	}
	if (m_pMotion->GetKey() >= 4)
	{
		if (m_pOrbit != NULL)
		{

			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	else
	{
		Mirage();
	}
	if (m_pMotion->GetKey() <= 5)
	{
		m_bMotionLock = true;
	}
	else
	{
		m_bMotionLock = false;
	}
	if (m_bKey)
	{
		m_bKey = false;
		D3DXVECTOR3 move = GetMove() + AnglesToVector(GetRot()) * -10.0f;
		move.y = 0.0f;
		SetMove(move);
	}
}

void CPlayer::Highroller()
{
	m_nDamage = 10;
	m_fPower = 0.0f;
	m_Size = 2.0f;
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
	pCamera->SetLenght(pCamera->GetLenght() + GetMove().y  + 10.0f);
	if (m_pColl != NULL)
	{
		m_pColl->SetKnockback(D3DXVECTOR3(m_pColl->GetKnockback().x, 15.0f, m_pColl->GetKnockback().z));
	}

	if (m_pMotion->GetKey() == 1)
	{
		if (m_pOrbit == NULL)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SLASHSWING);
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_DESTRUCT);
			m_pOrbit = COrbit::Create(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -55.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress(), "data\\TEXTURE\\OrbitBrade.png");
		}
	}
	
	if (m_pMotion->GetKey() >= 4)
	{
		if (m_pOrbit != NULL)
		{
		
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	else
	{
		Mirage();
	}
	if (m_pMotion->GetKey() <= 5)
	{
		m_bMotionLock = true;
	}
	else
	{
		m_bMotionLock = false;
	}
	CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();

	if (m_bKey && m_pMotion->GetKey() == 2 && pInputGamePad->GetPress(CInputGamePad::Button_Y,0))
	{
		m_bKey = false;
	
		SetMove(D3DXVECTOR3(GetMove().x, 13.0f, GetMove().z));
	}
}
void CPlayer::Split()
{
	m_nDamage = 10;
	m_fPower = 5.0f;
	m_Size = 3.0f;
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();

	if (m_pColl != NULL)
	{
		m_pColl->SetKnockback(D3DXVECTOR3(m_pColl->GetKnockback().x, -10.0f, m_pColl->GetKnockback().z));
	}

	if (m_pMotion->GetKey() == 1)
	{
		if (m_pOrbit == NULL)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SLASHSWING);
		
			m_pOrbit = COrbit::Create(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -55.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress(), "data\\TEXTURE\\OrbitBrade.png");
		}
	}

	if (m_pMotion->GetKey() >= 4)
	{
		if (m_pOrbit != NULL)
		{
		
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	else
	{
		Mirage();
	}
	if (m_pMotion->GetKey() <= 5)
	{
		m_bMotionLock = true;
	}
	else
	{
		m_bMotionLock = false;
	}
	CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();

	if (m_pMotion->GetKey() < 3)
	{
		pCamera->SetLenght(300.0f);
		SetMove(D3DXVECTOR3(GetMove().x, 1.0f, GetMove().z));
	}
	if (m_pMotion->GetKey() == 3)
	{
		SetMove(D3DXVECTOR3(GetMove().x, -15.0f, GetMove().z));
	}
	if (m_bLand && m_pMotion->GetKey() < 4)
	{
		CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_DESTRUCT);
		m_pMotion->SetkeyNumber(4);
	}
}

void CPlayer::Down()
{
	if (m_pMotion->GetKey() <= 5)
	{
		m_bMotionLock = true;
	}
	else
	{
		m_bMotionLock = false;
	}
}
void CPlayer::Dodge()
{

	if (m_pMotion->GetKey() <=4)
	{
		CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
		D3DXVECTOR3 vec = VECTO3ZERO;
		D3DXVECTOR3 rot = pCamera->GetRot();
		CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
		vec.x = pInputGamePad->GetStickL(0, 0.01f).x;
		vec.z = -pInputGamePad->GetStickL(0, 0.01f).y;
		D3DXMATRIX RotMtx;
		D3DXMatrixIdentity((&RotMtx));
		D3DXMatrixRotationY(&RotMtx, rot.y);
		D3DXVec3TransformCoord(&vec, &vec, &RotMtx);
		vec *= -1;
		SetMove(GetMove() + vec * 0.5f);
		m_bMotionLock = true;
		Mirage();
	}
	else
	{
		m_bMotionLock = false;
	}
}
void CPlayer::AutoCollisionCreate()
{
	if (m_pOrbit != NULL)
	{
		DeletCollision();
		D3DXVECTOR3 posold = m_pOrbit->GetPosOld();
		D3DXVECTOR3 pos = m_pOrbit->GetPos();
		D3DXVECTOR3 Vec = -AnglesToVector(GetRot()) *m_fPower;
		Vec.y += 3.0f;
		float Radius = CManager::GetInstance()->GetDistance(pos - posold) *m_Size;
		m_pColl = CSphereCollision::Create(CSphereCollision::TYPE_PLAYERATTACK,Radius, m_nDamage, pos,Vec,NULL,this);
	}
}
void CPlayer::DeletCollision()
{
	if (m_pColl != NULL)
	{
		delete m_pColl;
		m_pColl = NULL;
	}
	
}
void CPlayer::Direction()
{
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
	D3DXVECTOR3 vec = VECTO3ZERO;
	D3DXVECTOR3 rot = VECTO3ZERO;
	if (*m_pEnemy == NULL)
	{

		if (pInputKeyboard->GetPress(DIK_W) || pInputKeyboard->GetPress(DIK_A) || pInputKeyboard->GetPress(DIK_S) || pInputKeyboard->GetPress(DIK_D))
		{
			if (pInputKeyboard->GetPress(DIK_W))
			{
				vec.z += 1.0f;
			}
			if (pInputKeyboard->GetPress(DIK_A))
			{
				vec.x -= 1.0f;
			}
			if (pInputKeyboard->GetPress(DIK_S))
			{
				vec.z -= 1.0f;
			}
			if (pInputKeyboard->GetPress(DIK_D))
			{
				vec.x += 1.0f;
			}

		}
		else
		{
			vec.x = pInputGamePad->GetStickL(0, 0.01f).x;
			vec.z = -pInputGamePad->GetStickL(0, 0.01f).y;
		}
		if (vec != VECTO3ZERO)
		{
			D3DXVec3Normalize(&vec, &vec);
			if (pCamera != NULL)
			{
				rot = pCamera->GetRot();

				D3DXMATRIX RotMtx;
				D3DXMatrixIdentity((&RotMtx));
				D3DXMatrixRotationY(&RotMtx, rot.y);
				D3DXVec3TransformCoord(&vec, &vec, &RotMtx);
				vec *= -1;
				//向きを反映

			}

			m_rotDest.y = atan2f(-vec.x, -vec.z);
		}
		else
		{
			float fDistance = FLT_MAX;
			float fDistancecheck = 0.0f;
			int nSize = CEnemy::EnemyList.GetNum();
			CEnemy * pEnemy = NULL;
			CEnemy * pEnemyCross = NULL;
			for (int i = 0; i < nSize; i++)
			{
				pEnemy = CEnemy::EnemyList.Get(i);
				if (pEnemy != NULL)
				{
					fDistancecheck = GetDistance(pEnemy->GetPos() - pCamera->GetPosV());
					if (500.0f > fDistancecheck)
					{
						if (fDistance > fDistancecheck)
						{
							fDistance = fDistancecheck;
							pEnemyCross = pEnemy;
						}
					}
				}
			}
			if (pEnemyCross != NULL)
			{
				vec = pEnemyCross->GetPos() - GetPos();
				float move =(atan2f(-vec.x, -vec.z) - m_rotDest.y) *0.1f;
				if (move < -D3DX_PI)
				{
					move += D3DX_PI * 2;
				}
				else if (move > D3DX_PI)
				{
					move += -D3DX_PI * 2;
				}
				m_rotDest.y = m_rotDest.y + move;	
			}
		}
	}
	else
	{
		vec = (*m_pEnemy)->GetPos() - GetPos();
		m_rotDest.y = atan2f(-vec.x, -vec.z);
	}
}
void CPlayer::Mirage()
{

	for (int i = 0; i < 14; i++)
	{
		if (m_apModel[i] != NULL)
		{
			CAfterImageObject::Create((char *)m_apModel[i]->GetName().c_str(), m_apModel[i]->GetMatrix(), D3DXCOLOR(0.6f, 0.8f, 0.3f, 0.5f),12);
		}

	}
}
bool CPlayer::Damage(int Damage, D3DXVECTOR3 Knockback)
{
	if (m_pMotion->GetType() == MOTION_DODGE)
	{
		m_nScore += SCORE_BORDER * 0.33f;
	}
	if (m_pMotion->GetType() != MOTION_DAMAGE &&m_pMotion->GetType() != MOTION_DODGE)
	{
		if (m_pOrbit != NULL)
		{

			m_pOrbit->end();
			m_pOrbit = NULL;
		}
		m_nLife -= Damage;
		SetMove(GetMove() + Knockback);
		m_pMotion->SetType(MOTION_DAMAGE);
		m_nScore *= 0.5f;
		return true;
	}
	return false;
}
void  CPlayer::ScoreUp()
{
	float bonus = GetActionLogCount();
	
	m_nScore += (SCORE_BORDER * 0.1f) * (bonus + 1) / (m_nScore / SCORE_BORDER + 1);
}
void  CPlayer::SetLog(MOTION motion)
{
	m_nMovecount = 0;
	m_vActionLog.push_back(motion);
	if (m_vActionLog.size() > 5)
	{
		m_vActionLog.erase(m_vActionLog.begin());
	}
}
int CPlayer::GetActionLogCount()
{
	if (!m_vActionLog.empty())
	{
		std::set<MOTION> Unique(m_vActionLog.begin(), m_vActionLog.end());
		return Unique.size();
	}
	return 0;
}
void CPlayer::Lockon()
{
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();

	
	
		int nSize = CEnemy::EnemyList.GetNum();
		float fAngle = FLT_MAX;
		float fDistance = FLT_MAX;
		float fDistancecheck = 0.0f;
		float fAngleCheck = 0.0f;
		CEnemy * pEnemy = NULL;
		CEnemy * pEnemyOld = *m_pEnemy;
		D3DXVECTOR3 PosOld = m_pTarget->GetPos();
		*m_pEnemy = NULL;
		for (int i = 0; i < nSize; i++)
		{
			pEnemy = CEnemy::EnemyList.Get(i);
			if (pEnemy != NULL)
			{
				if (pEnemyOld == pEnemy)
				{
					*m_pEnemy = pEnemy;
					m_pTarget->SetPos(PosOld);
					break;
				}
				fAngleCheck = ComparisonAngle(-pCamera->GetVec(), pEnemy->GetPos() - pCamera->GetPosV());
				if (fAngleCheck < 45.0f)
				{
					if (fAngle > fAngleCheck)
					{
						fDistancecheck = GetDistance(pEnemy->GetPos() - pCamera->GetPosV());
						if (fDistance > fDistancecheck &&1500.0f > fDistancecheck)
						{
							fDistance = fDistancecheck;
							fAngle = fAngleCheck;
							*m_pEnemy = pEnemy;
							D3DXMATRIX mtx = (*m_pEnemy)->GetModel()->GetMatrix();

							m_pTarget->SetPos(D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
						}
					
					}
				}
			}
		}
	
	if (*m_pEnemy != NULL)
	{
		//敵とプレイヤーの座標をスクリーン座標に変換し、カメラの位置を決める
		CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
		LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
		pDevice = pRenderer->GetDevice();
		D3DXVECTOR3 EnemyPos = (*m_pEnemy)->GetPos();
		D3DXVECTOR3 PrayerPos = GetPos();
		EnemyPos += (*m_pEnemy)->GetModel()->GetPos();
		PrayerPos += m_apModel[0]->GetPos();
		D3DXVECTOR3 screenPosEnemy;
		D3DXVECTOR3 screenPosPlayer;
		D3DXMATRIX viewMatrix;
		D3DXMATRIX worldMtx;
		D3DXMatrixIdentity(&worldMtx);
		pDevice->GetTransform(D3DTS_VIEW, &viewMatrix);
		D3DVIEWPORT9 viewport;
		pDevice->GetViewport(&viewport);
		D3DXVec3Project(&screenPosEnemy, &EnemyPos, &viewport, &pCamera->m_mtxProjection,&pCamera->m_mtxView, &worldMtx);
		D3DXVec3Project(&screenPosPlayer, &PrayerPos, &viewport, &pCamera->m_mtxProjection, &pCamera->m_mtxView, &worldMtx);
		D3DXVECTOR3 pos = (*m_pEnemy)->GetPos();
		pos.y += 50.0f;
	
		D3DXVECTOR3 CameraPos = screenPosEnemy - screenPosPlayer;
		CameraPos.y *= 1.777f;
		CameraPos.z = 0.0f;
		float fLength = CManager::GetInstance()->GetDistance(CameraPos) ;
		if (fLength < 100.0f)
		{
			fLength = 100.0f;
		}
		else if (fLength > 1500.0f)
		{
			fLength = 1500.0f;
			*m_pEnemy = NULL;
			return;
		}
		D3DXVECTOR3 vec = pos - GetPos();
		if (m_pMotion->GetType() < MOTION_COMBINATION1)
		{
			pCamera->SetRDest((GetPos() + pos) * 0.5f);
			pCamera->SetLenght(pCamera->GetLenght() + (fLength - pCamera->GetLenght()) * 0.10f);
			//pCamera->SetRotDest(pCamera->GetRotDest() + (D3DXVECTOR3(0.0f, atan2f(-vec.x, -vec.z) - 0.2f, pCamera->GetRotDest().z) - pCamera->GetRotDest()) * 0.1f);
		}
		else
		{
			pCamera->SetRDest((GetPos() + pos) * 0.5f);
		}
	
		m_pTarget->SetHeight(m_pTarget->GetHeight() + (25.0f - m_pTarget->GetHeight()) * 0.2f);
		m_pTarget->SetWidth(m_pTarget->GetWidth() + (25.0f - m_pTarget->GetWidth()) * 0.2f);
		m_pTarget->SetColor(m_pTarget->GetColor() + (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) - m_pTarget->GetColor()) * 0.2f);
		m_pTarget->SetPos(m_pTarget->GetPos() + (pos - m_pTarget->GetPos()) * 0.3f);
	}
}
//=============================================
//生成関数
//=============================================
CPlayer * CPlayer::Create(void)
{
	CPlayer * pPlayer = NULL;
	pPlayer = DBG_NEW CPlayer;
	pPlayer->Init();

	return pPlayer;
}
