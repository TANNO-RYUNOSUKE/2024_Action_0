//========================================================================================
//
// �V�[��
// Author: �O�� ���V��
//
//========================================================================================
//�C���N���[�h
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "scene.h"

#include "camera.h"
#include "light.h"
#include "animbillboard.h"

#include "player.h"
#include "input.h"

#include "sound.h"
#include "debugproc.h"	
#include "fade.h"
#include "meshfield.h"
#include "collision.h"
#include "list.h"
#include "objectX.h"
#include "texture.h"
#include "audience.h"
#include "effect.h"
#include "lifegage.h"
CFade * CScene::m_pFade = NULL;
//=============================================
//�R���X�g���N�^
//=============================================
CScene::CScene()
{
	m_Mode = MODE_NONE;
	m_nFrame = 0;
}
//=============================================
//�f�X�g���N�^
//=============================================
CScene::~CScene()
{
}
//=============================================
//�����֐�
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
//�^�C�g��
//=================================================================================================
//=============================================
//�R���X�g���N�^
//=============================================
CTitle::CTitle()
{
}

CTitle::~CTitle()
{
}


//=============================================
//�������֐�
//=============================================
HRESULT CTitle::Init()
{
	m_pFade = DBG_NEW CFade;
	m_pFade->Init();
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH /2, SCREEN_HEIGHT/2, 0.0f),SCREEN_HEIGHT,SCREEN_WIDTH,0, "data\\TEXTURE\\TITLE\\ss.png");
	
	CSound * pSound = CManager::GetInstance()->GetSound();
	//pSound->Play(CSound::SOUND_LABEL_BGM001);
	return S_OK;
}
//=============================================
//�I���֐�
//=============================================
void CTitle::Uninit()
{

}
//=============================================
//�X�V�֐�
//=============================================
void CTitle::Update()
{
	
	
	CInputKeyboard * pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad * pInputGamepad = CManager::GetInstance()->GetInputGamePad();
	if (pInputKeyboard->GetTrigger(DIK_RETURN)|| pInputGamepad->GetTrigger(CInputGamePad::Button_START,0))
	{
		m_pFade->FadeOut(MODE::MODE_GAME);
	}
}
//=============================================
//�`��֐�
//=============================================
void CTitle::Draw()
{
	
}
//=================================================================================================
//�`���[�g���A��
//=================================================================================================
//=============================================
//�R���X�g���N�^
//=============================================
CTutorial::CTutorial()
{
}

CTutorial::~CTutorial()
{
}


//=============================================
//�������֐�
//=============================================
HRESULT CTutorial::Init()
{
	m_pFade = DBG_NEW CFade;
	m_pFade->Init();

	CSound * pSound = CManager::GetInstance()->GetSound();

	return S_OK;
}
//=============================================
//�I���֐�
//=============================================
void CTutorial::Uninit()
{

}
//=============================================
//�X�V�֐�
//=============================================
void CTutorial::Update()
{
	
	
	
}
//=============================================
//�`��֐�
//=============================================
void CTutorial::Draw()
{

}
//=================================================================================================
//�Q�[��
//=================================================================================================



CGame::CGame()
{
}

CGame::~CGame()
{
}
//=============================================
//�������֐�
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
	m_nEnemyCount = 0;
	CTexture * pTex = CManager::GetInstance()->GetTexture();
	//�������ݒ�;
	
	if (FAILED(m_pCamera->Init()))
	{
		return -1;
	};
	if (FAILED(m_pLight->Init()))
	{
		return -1;
	};
	CObjectX * pSky = CObjectX::Create("data\\MODEL\\sky.x", VECTO3ZERO, VECTO3ZERO,0);
	pSky->SetLight(false);
	pSky->m_bShadow = false;
	CObjectX::Create("data\\MODEL\\city.x", D3DXVECTOR3(0.0f,-100.0f,0.0f), VECTO3ZERO, 0);
	pTex->Regist("data\\TEXTURE\\spelhit.png");
	pTex->Regist("data\\TEXTURE\\HitEffect.png");
	pTex->Regist("data\\TEXTURE\\HitEffect2.png");

	CEnemy_army::Create(D3DXVECTOR3(0.0f,0.0f,500.0f), 150);
	CEnemy_Boss::Create(D3DXVECTOR3(0.0f, 0.0f, -1500.0f), 150);

	CSound * pSound = CManager::GetInstance()->GetSound();
	pSound->Play(CSound::SOUND_LABEL_BGM_ZONE);
	CGage::Create(D3DXVECTOR3(65.0f, 65.0f, 0.0f), 377.0f, PLAYERLIFE_MAX);

	CObject2D::Create(D3DXVECTOR3(250.0f, 60.0f, 0.0f), 59.0f, 413.0f, 6, "data\\TEXTURE\\life.png");

	return S_OK;
	
}
//=============================================
//�I���֐�
//=============================================
void CGame::Uninit()
{

	if (m_pCamera != NULL)
	{
		//���������
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	if (m_pLight != NULL)
	{
		//���������
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}


	CSphereCollision::List.GetList()->clear();
}
//=============================================
//�X�V�֐�
//=============================================
void CGame::Update()
{
	CDebugProc * pDeb = CManager::GetInstance()->GetDeb();
	if (!CManager::GetInstance()->GetPause())
	{
		m_nCnt++;
		if (m_nCnt % (5 * 60) == 0 && m_nEnemyCount < 6)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3((rand() % 10000 -5000) * 0.1f, 0.0f, (rand() % 10000 - 5000) * 0.1f);
			CEnemy_army::Create(pos, 200);
			CAnimBillboard::Create(200.0f, 200.0f, 6, 6, 36, 24, false, pos, "data\\TEXTURE\\spelhit.png");
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
			m_nEnemyCount++;
		}
		if (m_nCnt % 10 == 0 && m_pPlayer != NULL)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3((rand() % 10000 - 5000) * 0.1f, (rand() %  5000) * 0.1f, (rand() % 10000 - 5000) * 0.1f) + m_pPlayer->GetPos();
			D3DXVECTOR3 move = D3DXVECTOR3((rand() % 10 - 5) * 0.1f, (rand() % 10) * 0.1f, (rand() % 10 - 5) * 0.1f);
			CEffect::Create(pos, move, rand() % 300 + 60, D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f), (rand() % 10) * 0.1f + 1.0f, (rand() % 100 - 50) * 0.0001f + 1.0f);
		}
	}
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
	if (m_nEnemyCount >= 3 && CEnemy::EnemyList.GetNum() == 0)
	{
		m_pFade->FadeOut(MODE::MODE_RESULT);
	}
	
	if (pInputKeyboard->GetTrigger(DIK_RETURN))
	{
		m_pFade->FadeOut(MODE::MODE_RESULT);
	}
	
}
//=============================================
//�`��֐�
//=============================================
void CGame::Draw()
{
	
}

//=============================================
//���W����
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
//�R���X�g���N�^
//=============================================
CResult::CResult()
{
}

CResult::~CResult()
{
}
//=============================================
//�������֐�
//=============================================
HRESULT CResult::Init()
{
	m_pFade = DBG_NEW CFade;
	m_pFade->Init();
	
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_HEIGHT, SCREEN_WIDTH, 0, "data\\TEXTURE\\Ranking.png");
	return S_OK;
}
//=============================================
//�I���֐�
//=============================================
void CResult::Uninit()
{
	
}
//=============================================
//�X�V�֐�
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
//�`��֐�
//=============================================
void CResult::Draw()
{
	
}
