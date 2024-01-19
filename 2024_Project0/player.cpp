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
	
	m_nLife = 5;
	m_rotDest = VECTO3ZERO;
	m_pMotion = DBG_NEW CMotion;
	m_pMotion->SetModel(&m_apModel[0]);
	m_pMotion->Load("data\\TEXT\\motion_player2024_0.txt");
	m_bMotionLock = false;
	m_bKey = false;
	m_pOrbit = NULL;
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CPlayer::Uninit()
{
	
	
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
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
	
	Action();

	SetPos(GetPos() + GetMove());
	SetMove(GetMove() * 0.9);
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
	if (m_pMotion->GetType() < MOTION_COMBINATION1)
	{
		m_bMotionLock = false;

		m_bKey = false;
		if (m_pOrbit != NULL)
		{
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	if (!m_bMotionLock && m_pMotion->GetType() < MOTION_COMBINATION1)
	{
		Walk();

	}
	if (!m_bMotionLock)
	{
		if (pInputMouse->GetTrigger(pInputMouse->MOUSE_LEFT))
		{
			if (m_pOrbit != NULL)
			{
				m_pOrbit->end();
				m_pOrbit = NULL;
			}
			
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
		vec = pInputGamePad->GetStickL(0, 0.01f);
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
void CPlayer::Attack1()
{
	if (m_pMotion->GetKey() == 2)
	{
		if (m_pOrbit == NULL)
		{
			m_pOrbit = COrbit::Create(30, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -54.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress());
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
		for (int i = 0; i < 14; i++)
		{
			if (m_apModel[i] != NULL)
			{
				CAfterImageObject::Create((char *)m_apModel[i]->GetName().c_str(), m_apModel[i]->GetMatrix(), D3DXCOLOR(0.6f, 0.8f, 0.3f, 0.5f), 60);
			}

		}
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
	if (m_pMotion->GetKey() == 2)
	{
		if (m_pOrbit == NULL)
		{
			m_pOrbit = COrbit::Create(30, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -44.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress());
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
		for (int i = 0; i < 14; i++)
		{
			if (m_apModel[i] != NULL)
			{
				CAfterImageObject::Create((char *)m_apModel[i]->GetName().c_str(), m_apModel[i]->GetMatrix(), D3DXCOLOR(0.6f, 0.8f, 0.3f, 0.5f), 60);
			}

		}
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
	if (m_pMotion->GetKey() == 1)
	{
		if (m_pOrbit == NULL)
		{
			m_pOrbit = COrbit::Create(30, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -44.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress());
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
		for (int i = 0; i < 14; i++)
		{
			if (m_apModel[i] != NULL)
			{
				CAfterImageObject::Create((char *)m_apModel[i]->GetName().c_str(), m_apModel[i]->GetMatrix(), D3DXCOLOR(0.6f, 0.8f, 0.3f, 0.5f), 60);
			}

		}
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
	if (m_pMotion->GetKey() == 2)
	{
		if (m_pOrbit == NULL)
		{
			m_pOrbit = COrbit::Create(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -60.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress());
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
		 for (int i = 0; i < 14; i++)
		 {
			 if (m_apModel[i] != NULL)
			 {
				 CAfterImageObject::Create((char *)m_apModel[i]->GetName().c_str(), m_apModel[i]->GetMatrix(), D3DXCOLOR(0.6f, 0.8f, 0.3f, 0.5f), 60);
			 }

		 }
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
