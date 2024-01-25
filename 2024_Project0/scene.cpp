//========================================================================================
//
// シーン
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "scene.h"

#include "camera.h"
#include "light.h"

#include "player.h"
#include "input.h"

#include "sound.h"
#include "debugproc.h"	
#include "fade.h"
#include "meshfield.h"
#include "collision.h"
#include "list.h"
#include "texture.h"
CFade * CScene::m_pFade = NULL;
//=============================================
//コンストラクタ
//=============================================
CScene::CScene()
{
	m_Mode = MODE_NONE;
}
//=============================================
//デストラクタ
//=============================================
CScene::~CScene()
{
}
//=============================================
//生成関数
//=============================================
CScene * CScene::Create(MODE mode)
{
	CScene * pScene = NULL;
	switch (mode)
	{
	case CScene::MODE_TITLE:
		pScene = DBG_NEW CTitle;
		break;
	case CScene::MODE_TUTORIAL:
		pScene = DBG_NEW CTutorial;
		break;
	case CScene::MODE_GAME:
		pScene = DBG_NEW CGame;
		break;
	case CScene::MODE_RESULT:
		pScene = DBG_NEW CResult;
		break;
	default:
		break;
	}

	pScene->SetMode(mode);
	pScene->Init();

	return pScene;
}
//=================================================================================================
//タイトル
//=================================================================================================
//=============================================
//コンストラクタ
//=============================================
CTitle::CTitle()
{
}

CTitle::~CTitle()
{
}


//=============================================
//初期化関数
//=============================================
HRESULT CTitle::Init()
{
	m_pFade = DBG_NEW CFade;
	m_pFade->Init();
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT/2, 0.0f),SCREEN_HEIGHT,SCREEN_WIDTH,0, "data\\TEXTURE\\TITLE\\lab1.png");
	m_pTitle = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.7f, 0.0f), 305.0f, 600.0f, 0, "data\\TEXTURE\\TITLE\\lab2.png");
	m_posDest = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.7f, 0.0f);
	CSound * pSound = CManager::GetInstance()->GetSound();
	//pSound->Play(CSound::SOUND_LABEL_BGM001);
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CTitle::Uninit()
{

}
//=============================================
//更新関数
//=============================================
void CTitle::Update()
{
	
	m_pTitle->SetPos(m_pTitle->GetPos() + (m_posDest - m_pTitle->GetPos()) / 120);
	if (CManager::GetInstance()->GetDistance((m_pTitle->GetPos() - m_posDest)) <= 10.0f)
	{
		m_posDest = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.7f, 0.0f);
	
		m_posDest.x += rand() % 100 - 50;
	}
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamepad = CManager::GetInstance()->GetInputGamePad();
	if (pInputKeyboard->GetTrigger(DIK_RETURN)|| pInputGamepad->GetTrigger(CInputGamePad::Button_START,0))
	{
		m_pFade->FadeOut(MODE::MODE_GAME);
	}
}
//=============================================
//描画関数
//=============================================
void CTitle::Draw()
{
	
}
//=================================================================================================
//チュートリアル
//=================================================================================================
//=============================================
//コンストラクタ
//=============================================
CTutorial::CTutorial()
{
}

CTutorial::~CTutorial()
{
}


//=============================================
//初期化関数
//=============================================
HRESULT CTutorial::Init()
{
	m_pFade = DBG_NEW CFade;
	m_pFade->Init();

	CSound * pSound = CManager::GetInstance()->GetSound();

	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CTutorial::Uninit()
{

}
//=============================================
//更新関数
//=============================================
void CTutorial::Update()
{
	
	
	
}
//=============================================
//描画関数
//=============================================
void CTutorial::Draw()
{

}
//=================================================================================================
//ゲーム
//=================================================================================================



CGame::CGame()
{
}

CGame::~CGame()
{
}
//=============================================
//初期化関数
//=============================================
HRESULT CGame::Init()
{
	b_Pause = false;
	m_nCnt = 0;
	m_pFade = DBG_NEW CFade;
	m_pFade->Init();
	m_pCamera = DBG_NEW CCamera;
	m_pLight = DBG_NEW CLight;
	m_pMeshfield = CMeshfield::Create(100.0f, 100.0f, 70, 70);
	m_pPlayer = CPlayer::Create();
	CTexture * pTex = CManager::GetInstance()->GetTexture();
	//初期化設定;
	
	if (FAILED(m_pCamera->Init()))
	{
		return -1;
	};
	if (FAILED(m_pLight->Init()))
	{
		return -1;
	};

	pTex->Regist("data\\TEXTURE\\spelhit.png");
	pTex->Regist("data\\TEXTURE\\HitEffect.png");
	CEnemy_TEST::Create(VECTO3ZERO, 100);

	CSound * pSound = CManager::GetInstance()->GetSound();
	pSound->Play(CSound::SOUND_LABEL_BGM_ZONE);


	return S_OK;
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CGame::Uninit()
{

	if (m_pCamera != NULL)
	{
		//メモリ解放
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	if (m_pLight != NULL)
	{
		//メモリ解放
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}


	CSphereCollision::List.GetList()->clear();
}
//=============================================
//更新関数
//=============================================
void CGame::Update()
{
	CDebugProc * pDeb = CManager::GetInstance()->GetDeb();
	m_nCnt++;

	m_pCamera->Update();
	m_pLight->Update();
	//m_pMap_Editer->Update();
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamepad = CManager::GetInstance()->GetInputGamePad();
	if (pInputKeyboard->GetTrigger(DIK_P) || pInputGamepad->GetTrigger(CInputGamePad::Button_START,0))
	{
		if (CManager::GetInstance()->GetPause())
		{
			CManager::GetInstance()->SetPause(false);
		}
		else
		{
			CManager::GetInstance()->SetPause(true);
		}
	}
	if (!CManager::GetInstance()->GetPause())
	{
		if (!CSphereCollision::List.GetList()->empty())
		{
			std::list<CSphereCollision *>::iterator Itr = CSphereCollision::List.GetList()->begin();
			for (; Itr != CSphereCollision::List.GetList()->end(); Itr++)
			{
				CSphereCollision * Col = *Itr;
				if (Col != NULL)
				{
					Col->Collision();
				}
			}
		}
	
	}
	if (m_nCnt % 900 == 0)
	{
		for (int nCnt = 0; nCnt < 5; nCnt++)
		{
			//CEnemy_Walker::Create(D3DXVECTOR3((float)(rand() % 3000 - 1500), 10000.0f, (float)(rand() % 3000 - 1500)), 5);
		}
	}
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputGamepad->GetTrigger(CInputGamePad::Button_START, 0))
	{
		m_pFade->FadeOut(MODE::MODE_RESULT);
	}
	
}
//=============================================
//描画関数
//=============================================
void CGame::Draw()
{
	
}

//=============================================
//座標制限
//=============================================
void CGame::positionLimiter(D3DXVECTOR3* pos)
{
	if (pos->x > MAX_X)
	{
		pos->x = MAX_X;
	}
	else if (pos->x < -MAX_X)
	{
		pos->x = -MAX_X;
	}

	if (pos->y > MAX_Y)
	{
		pos->y = MAX_Y;
	}
	

	if (pos->z > MAX_Z)
	{
		pos->z = MAX_Z;
	}
	else if (pos->z < -MAX_Z)
	{
		pos->z = -MAX_Z;
	}
}
//=================================================================================================
//result
//=================================================================================================
//=============================================
//コンストラクタ
//=============================================
CResult::CResult()
{
}

CResult::~CResult()
{
}
//=============================================
//初期化関数
//=============================================
HRESULT CResult::Init()
{
	m_pFade = DBG_NEW CFade;
	m_pFade->Init();
	
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH, 0, "data\\TEXTURE\\Ranking.png");
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CResult::Uninit()
{
	
}
//=============================================
//更新関数
//=============================================
void CResult::Update()
{

	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamepad = CManager::GetInstance()->GetInputGamePad();
	if (pInputKeyboard->GetTrigger(DIK_RETURN)|| pInputGamepad->GetTrigger(CInputGamePad::Button_START, 0))
	{
		m_pFade->FadeOut(MODE::MODE_TITLE);
	}
}
//=============================================
//描画関数
//=============================================
void CResult::Draw()
{
	
}
