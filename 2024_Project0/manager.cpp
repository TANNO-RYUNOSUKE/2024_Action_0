//========================================================================================
//
// マネージャ
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "object2D.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "objectX.h"
#include "billboard.h"
#include "texture.h"
#include "scene.h"
#include "xfile.h"
#include "Xmanager.h"

CManager * CManager::pInstance = NULL;
//=============================================
//コンストラクタ
//=============================================
CManager::CManager()
{
	 m_pRenderer = NULL;
	m_pInputKeyboard = NULL;
	m_pInputGamepad = NULL;
	m_pInputMouse = NULL;
	m_pSound = NULL;
	m_pDeb = NULL;
	m_pTexture = NULL;
	m_pXFile = NULL;
	m_pScene = NULL;
	m_pItemManager = NULL;
	m_pEnemyManager = NULL;
	m_pXManager = NULL;
	m_bPause = false;
	m_nHitStop = 0;
	m_nScoreSave = 0;
	
}
//=============================================
//デストラクタ
//=============================================
CManager::~CManager()
{
}

//=============================================
//初期化関数
//=============================================
HRESULT CManager::Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow)
{
	m_bPause = false;
	m_pRenderer = DBG_NEW CRenderer;
	m_pInputKeyboard = DBG_NEW CInputKeyboard;
	m_pInputGamepad = DBG_NEW CInputGamePad;
	m_pInputMouse = DBG_NEW CInputMouse;
	m_pDeb = DBG_NEW CDebugProc;
	m_pSound = DBG_NEW CSound;
	m_pTexture = DBG_NEW CTexture;
	m_pXFile = DBG_NEW CXFile;

	m_pXManager = DBG_NEW CXManager;
	m_pXManager->Init();
	//初期化設定;
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return -1;
	};
	if (FAILED(m_pInputKeyboard->Init(hinstance,hWnd)))
	{
		return -1;
	};
	if (FAILED(m_pInputGamepad->Init(hinstance, hWnd)))
	{
		return -1;
	};
	if (FAILED(m_pInputMouse->Init(hinstance, hWnd)))
	{
		return -1;
	};
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return -1;
	};



	m_pDeb->Init();


	
	



	SetMode(CScene::MODE::MODE_TITLE);

	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CManager::Uninit()
{

	CObject::ReleaseAll();
	
	m_pSound->Stop();

	if (m_pRenderer != NULL)
	{//メモリ解放
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	if (m_pInputKeyboard != NULL)
	{//メモリ解放
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	if (m_pInputGamepad != NULL)
	{
		//メモリ解放
		m_pInputGamepad->Uninit();
		delete m_pInputGamepad;
		m_pInputGamepad = NULL;
	}
	if (m_pInputMouse != NULL)
	{
		//メモリ解放
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}
	if (m_pSound != NULL)
	{
		//メモリ解放
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}
	if (m_pDeb != NULL)
	{
		//メモリ解放
		m_pDeb->Uninit();
		delete m_pDeb;
		m_pDeb = NULL;
	}
	if (m_pTexture != NULL)
	{
		//メモリ解放
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = NULL;
	}
	if (m_pXFile != NULL)
	{
		//メモリ解放
		m_pXFile->Unload();
		delete m_pXFile;
		m_pXFile = NULL;
	}
	
	if (m_pXManager != NULL)
	{
		//メモリ解放
		delete m_pXManager;
		m_pXManager = NULL;
	}
}
//=============================================
//更新関数
//=============================================
void CManager::Update()
{
	//更新処理
	m_pInputKeyboard->Update();
	m_pInputGamepad->Update();
	m_pInputMouse->Update();
	if (m_bPause != true && m_nHitStop <= 0)
	{
		m_pRenderer->Update();
		m_pDeb->Update();
	}
	else
	{
		m_pScene->Update();
	}
	m_nHitStop--;
}
//=============================================
//描画関数
//=============================================
void CManager::Draw()
{
	//更新処理

	m_pRenderer->Draw();
	
}
//=============================================
//モード設定
//=============================================
void CManager::SetMode(CScene::MODE mode)
{
	m_bPause = false;
	m_nHitStop = 0;
	m_pSound->Stop();
	if (m_pScene != NULL)
	{
		m_pScene->Uninit();
	}
	
	CObject::ReleaseAll();
	m_pScene = CScene::Create(mode);
}
//=============================================
//ポリゴンへのレイ関数
//=============================================
bool CManager::RayTri(const D3DXVECTOR3& startPoint, const D3DXVECTOR3& endPoint, const D3DXVECTOR3& polypos1, const D3DXVECTOR3& polypos2, const D3DXVECTOR3& polypos3,D3DXVECTOR3 * pPos)
{
	// レイの作成
	D3DXVECTOR3 rayOrigin = startPoint;
	D3DXVECTOR3 rayDirection = endPoint - startPoint;
	bool isIntersecting = false;
	float u, v, distance;
	distance = -1.0f;
	D3DXIntersectTri(
		&polypos1,          // ポリゴンの頂点0の座標
		&polypos2,          // ポリゴンの頂点1の座標
		&polypos3,          // ポリゴンの頂点2の座標
		&rayOrigin,         // レイの始点の座標
		&rayDirection,      // レイの方向ベクトル
		&u,                 // 交差点の重み u
		&v,                 // 交差点の重み v
		&distance           // レイとポリゴンの距離
	);

	// レイとポリゴンが交差しているかどうかを判定
	if (distance >= 0.0f && distance <= 1.0f)
	{
		// レイとポリゴンが交差している
		isIntersecting = true;
		if (pPos != NULL)
		{
			*pPos = rayOrigin + rayDirection * distance;
		}
	
	}
	else
	{
		// レイとポリゴンが交差していない
		isIntersecting = false;
	}
	return isIntersecting;
}

//=============================================
//距離取得関数
//=============================================
float CManager::GetDistance(D3DXVECTOR3 vec)
{
	if (vec.x < 0.0f)
	{
		vec.x *= -1.0f;
	}
	if (vec.y < 0.0f)
	{
		vec.y *= -1.0f;
	}
	if (vec.z < 0.0f)
	{
		vec.z *= -1.0f;
	}
	float fDis = vec.x + vec.y + vec.z;
	return fDis;
}
//=============================================
//レンダラーの取得
//=============================================
CRenderer * CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//=============================================
//キーボード取得
//=============================================
CInputKeyboard * CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}
//=============================================
//コントローラー取得
//=============================================
CInputGamePad * CManager::GetInputGamePad(void)
{
	return m_pInputGamepad;
}
//=============================================
//マウス取得
//=============================================
CInputMouse * CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}

//=============================================
//デバッグプロシージャ取得
//=============================================
CDebugProc * CManager::GetDeb(void)
{
	return m_pDeb;
}