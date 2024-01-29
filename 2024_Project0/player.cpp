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

	SetType(CObject::TYPE_PLAYER);

	m_nLife = 0;



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
	m_pTarget = CBillboard::Create(50.0f, 50.0f, GetPos(), "data\\TEXTURE\\target.png");
	m_nLife = 5;
	m_pEnemy = DBG_NEW CEnemy*;
	m_rotDest = VECTO3ZERO;
	m_pMotion = DBG_NEW CMotion;
	m_pMotion->SetModel(&m_apModel[0]);
	m_pMotion->Load("data\\TEXT\\motion_player2024_0.txt");
	m_bMotionLock = false;
	m_bKey = false;
	m_pOrbit = NULL;
	m_pColl = NULL;
	m_nDamage = 0;
	m_fPower = 0.0f;
	m_Size = 0.0f;
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
		m_pOrbit->Release();
		m_pOrbit = NULL;
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
	m_posOld = GetPos();
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
	CInputMouse * pInputMouse = CManager::GetInstance()->GetInputMouse();
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
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
	
	SetPos(GetPos() + GetMove());
	SetMove(GetMove() * 0.9f);
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
		if (pMesh->Collision(GetPosAddress()))
		{
			
				move.y = 0.0f;
				m_bLand = true;
		}
		else
		{
			move.y -= GRAVITY;
			m_bLand = false;
		}
		SetMove(move);
	}
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

void CPlayer::Action()
{
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
	CInputMouse * pInputMouse = CManager::GetInstance()->GetInputMouse();
	m_pMotion->Update();
	AutoCollisionCreate();
	if (m_pMotion->GetType() < MOTION_COMBINATION1)
	{
		m_bMotionLock = false;

		m_bKey = false;
		DeletCollision();
		if (m_pOrbit != NULL)
		{
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	if (!m_bMotionLock && m_pMotion->GetType() < MOTION_COMBINATION1)
	{
		CInputGamePad * pInputGamePad = CManager::GetInstance()->GetInputGamePad();
		D3DXVECTOR3 vec = pInputGamePad->GetStickL(0,0.1f);

		if (CManager::GetInstance()->GetDistance(vec) <= 0.9f || pInputKeyboard->GetPress(DIK_LSHIFT))
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
			if (m_pMotion->GetType() < MOTION_COMBINATION1)
			{
				m_pMotion->SetType(MOTION_COMBINATION1);

			}
			else if (m_pMotion->GetType() < MOTION_MAX - 1)
			{
				m_pMotion->SetType(m_pMotion->GetType() + 1);
			}
		}
	}
	switch (m_pMotion->GetType())
	{
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
	}
	
}
void CPlayer::Attack2()
{
	m_nDamage = 8;
	m_fPower = 10.0f;
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
void CPlayer::AutoCollisionCreate()
{
	if (m_pOrbit != NULL)
	{
		DeletCollision();
		D3DXVECTOR3 posold = m_pOrbit->GetPosOld();
		D3DXVECTOR3 pos = m_pOrbit->GetPos();
		D3DXVECTOR3 Vec = -AnglesToVector(GetRot()) *m_fPower;
		Vec.y += 10.0f;
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
			CAfterImageObject::Create((char *)m_apModel[i]->GetName().c_str(), m_apModel[i]->GetMatrix(), D3DXCOLOR(0.6f, 0.8f, 0.3f, 0.5f), 60);
		}

	}
}
void CPlayer::Lockon()
{
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
	if (*m_pEnemy == NULL)
	{
	
		int nSize = CEnemy::EnemyList.GetNum();
		float fAngle = FLT_MAX;
		float fAngleCheck = 0.0f;
		CEnemy * pEnemy = NULL;

		for (int i = 0; i < nSize; i++)
		{
			pEnemy = CEnemy::EnemyList.Get(i);
			if (pEnemy != NULL)
			{
				fAngleCheck = ComparisonAngle(-pCamera->GetVec(), pEnemy->GetPos() - pCamera->GetPosV());
				if (fAngleCheck < 45.0f)
				{
					if (fAngle > fAngleCheck)
					{
						fAngle = fAngleCheck;
						*m_pEnemy = pEnemy;
						m_pTarget->SetPos((*m_pEnemy)->GetPos());
					}
				}
			}
		}
	}
	else
	{
		
		D3DXVECTOR3 pos = (*m_pEnemy)->GetPos();
		pos.y += 50.0f;
		pCamera->SetRDest((GetPos()+ pos) * 0.5f);
		float fLength = CManager::GetInstance()->GetDistance((GetPos() + pos)) *0.5f;
		if (fLength < 300.0f)
		{
			fLength = 300.0f;
		}
		D3DXVECTOR3 vec = pos - GetPos();
		pCamera->SetLenght(fLength);
		pCamera->SetRot(D3DXVECTOR3(atan2f(-vec.y, -vec.z) + 0.5f, atan2f(-vec.x, -vec.z),0.0f));
		m_pTarget->SetHeight(m_pTarget->GetHeight() + (50.0f - m_pTarget->GetHeight()) * 0.2f);
		m_pTarget->SetWidth(m_pTarget->GetWidth() + (50.0f - m_pTarget->GetWidth()) * 0.2f);
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
