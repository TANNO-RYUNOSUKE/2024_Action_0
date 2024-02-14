//========================================================================================
//
// レンダラー
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "object.h"
#include "debugproc.h"
#include "manager.h"
#include "ZTexture.h"
#include "DepthShadow.h"
#include "player.h"
#include "shader.h"
//=============================================
//コンストラクタ
//=============================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

//=============================================
//デストラクタ
//=============================================
CRenderer::~CRenderer()
{
}

//=============================================
//初期化関数
//=============================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	//DirectXオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	ZeroMemory(&d3dpp, sizeof(d3dpp)); //パラメータのゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct#Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//カリングの設定
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	//サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//各種オブジェクト初期化処理はここ
	
	m_pD3DDevice->CreateTexture(
		(SCREEN_WIDTH),          // テクスチャの幅
		(SCREEN_HEIGHT),          // テクスチャの高さ
		0,              // ミップマップ レベル
		D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8, // フォーマット
		D3DPOOL_DEFAULT, // プール
		&pSceneTexture,  // テクスチャ オブジェクト
		NULL);
	m_pD3DDevice->CreateTexture(
		(160),          // テクスチャの幅
		(90),          // テクスチャの高さ
		0,              // ミップマップ レベル
		D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8, // フォーマット
		D3DPOOL_DEFAULT, // プール
		&pBloomTexture,  // テクスチャ オブジェクト
		NULL);
	m_pScreen = CObject2D::Create(SCREEN_CENTER, SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pScreen->m_bDraw = false;
	m_pScreen->m_bAutoRelease = false;
	m_pScreen->SetTex(pSceneTexture);
	m_pBloom = CObject2D::Create(SCREEN_CENTER, SCREEN_HEIGHT, SCREEN_WIDTH);
	m_pBloom->m_bDraw = false;
	m_pBloom->m_bAutoRelease = false;
	m_pBloom->SetTex(pBloomTexture);
	m_pBlomMini = CObject2D::Create(VECTO3ZERO, SCREEN_HEIGHT / 8, SCREEN_WIDTH / 8, 0, NULL, D3DXVECTOR2(0.0f, 0.0f));
	m_pBlomMini->m_bDraw = false;
	m_pBlomMini->m_bAutoRelease = false;
	m_pBlomMini->SetTex(pBloomTexture);


	Vdest = {};
	Rdest = {};
	V = {};
	R = {};
	
		V = (D3DXVECTOR3(0.0f, 500.0f, 0.0f));
		R = (D3DXVECTOR3(10.0f, -10.0f, 0.0f));
	
	
	// シェーダー用の初期化は以下
		Loadshader("data\\SHADER\\BrightCheck.fx", &pEffectBrightCheck);
		Loadshader("data\\SHADER\\Gauss.fx", &pGauss);
	// Z値テクスチャ生成オブジェクトの生成と初期化
	m_pDev = m_pD3DDevice;
	D3DXCreateSprite(m_pD3DDevice, &m_pSprite);// スプライト作成
	m_pZTex = DBG_NEW CZTexture;
	m_pZTex->Init(*m_pDev, SCREEN_WIDTH * 1.0f, SCREEN_WIDTH * 1.0f, D3DFMT_A32B32G32R32F);
	m_pZTex->GetZTex(&m_pZTexture);

	// 深度バッファシャドウオブジェクトの生成と初期化
	m_pDepthShadow = DBG_NEW CDepthShadow;
	m_pDepthShadow->Init(*m_pDev);
	m_pDepthShadow->SetShadowMap(&m_pZTexture);	// シャドウマップテクスチャを登録
	D3DXMatrixPerspectiveFovLH(&CameraProj, D3DXToRadian(45), 1.777f, 10.0f, 50000.0f);
	D3DXMatrixPerspectiveFovLH(&LightProj, D3DXToRadian(45), 1.0f, 30.0f, 50000.0f);
	D3DXMatrixLookAtLH(&LightView, &D3DXVECTOR3(00.0f, 500.0f, 000.0f), &D3DXVECTOR3(10.0f, -10.0f, 00.0f), &D3DXVECTOR3(0, 1, 0));

	// Z値テクスチャOBJへ登録
	m_pZTex->SetViewMatrix(&LightView);
	m_pZTex->SetProjMatrix(&LightProj);

	// 深度バッファシャドウOBJへ登録
	// カメラビューは毎回変わるので描画時に登録します
	m_pDepthShadow->SetLightViewMatrix(&LightView);
	m_pDepthShadow->SetLightProjMatrix(&LightProj);
	m_pDepthShadow->SetCameraProjMatrix(&CameraProj);

	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CRenderer::Uninit(void)
{
	//各種オブジェクトの終了処理
	
	//DirectX3dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	//DirectX3dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
	
	if (m_pDepthShadow != NULL)
	{
		delete m_pDepthShadow;
		m_pDepthShadow = NULL;
	}
	if (m_pZTex != NULL)
	{
		delete m_pZTex;
		m_pZTex = NULL;
	}
}
//=============================================
//更新関数
//=============================================
void CRenderer::Update(void)
{
	//各種オブジェクトの更新処理
	CObject::UpDateAll();
	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	if (pPlayer != NULL)
	{
		 Vdest = (D3DXVECTOR3(0.0f, 750.0f, 0.0f) + pPlayer->GetPos());
		 Rdest = (D3DXVECTOR3(10.0f, -10.0f, 0.0f) + pPlayer->GetPos());
		 V = (V + (Vdest - V)* 0.01f);
		 R = (R + (Rdest - R)* 0.1f);
		D3DXMatrixLookAtLH(&LightView, &V, &R, &D3DXVECTOR3(0, 1, 0));
		m_pDepthShadow->SetLightpos(V);
		m_pDepthShadow->SetLightSeepos(R);
		// Z値テクスチャOBJへ登録
		m_pZTex->SetViewMatrix(&LightView);
		// 深度バッファシャドウOBJへ登録

		m_pDepthShadow->SetLightViewMatrix(&LightView);
	
	
	}
}
//=============================================
//描画関数
//=============================================
void CRenderer::Draw(void)
{
	CDebugProc * pDeb = CManager::GetInstance()->GetDeb();
	//描画サーフェイス取得
	IDirect3DSurface9* pBackBuffer;
	m_pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	IDirect3DSurface9* pRenderTarget;
	IDirect3DSurface9* pBloomBuffer;
	pSceneTexture->GetSurfaceLevel(0, &pRenderTarget);
	pBloomTexture->GetSurfaceLevel(0, &pBloomBuffer);
	m_pD3DDevice->SetRenderTarget(0, pBackBuffer);
	//画面クリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//成功した場合
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		m_pZTex->Begin();
		CObject::Draw3D();
		m_pZTex->End();
		m_pDepthShadow->Begin();
		CObject::Draw3D();
		m_pDepthShadow->End();
		// オブジェクトの全描画
		CObject::Draw3D();

		// テクスチャサンプリングステートを設定
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP); // U方向のラッピングを無効化
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP); // V方向のラッピングを無効化

		// 一定以上の輝度を抜き出す

		m_pD3DDevice->SetRenderTarget(0, pBloomBuffer);

		m_pZTex->Begin();
		CObject::Draw3D();
		m_pZTex->End();
		m_pDepthShadow->Begin();
		CObject::Draw3D();
		m_pDepthShadow->End();
		// オブジェクトの全描画
		CObject::Draw3D();

		m_pD3DDevice->SetRenderTarget(0, pBloomBuffer);
		// 固定機能に戻す
		m_pD3DDevice->SetVertexShader(NULL);
		m_pD3DDevice->SetPixelShader(NULL);
		pEffectBrightCheck->Begin(NULL, 0);
		pEffectBrightCheck->BeginPass(0);
		pEffectBrightCheck->SetTechnique("brightcheck");
	
		m_pBlomMini->Draw();
	
		pEffectBrightCheck->EndPass();
		pEffectBrightCheck->End();
		// 固定機能に戻す
		m_pD3DDevice->SetVertexShader(NULL);
		m_pD3DDevice->SetPixelShader(NULL);


		m_pD3DDevice->SetRenderTarget(0, pRenderTarget);
		// ガウスぼかしを適用
		pGauss->Begin(NULL, 1);
		pGauss->BeginPass(0);
		pGauss->SetTechnique("GaussianBlur");
		pGauss->SetTexture("InputTexture", pBloomTexture);
		m_pBloom->Draw();
		pGauss->EndPass();
		pGauss->End();
		// 固定機能に戻す
		m_pD3DDevice->SetVertexShader(NULL);
		m_pD3DDevice->SetPixelShader(NULL);
	


		m_pD3DDevice->SetRenderTarget(0,pBackBuffer);
		m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pScreen->Draw();
		m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		CObject::DrawUI();
		D3DXMATRIX SpriteScaleMat;
		D3DXMatrixScaling(&SpriteScaleMat, 0.125f, 0.125f, 1.0f);
		m_pSprite->SetTransform(&SpriteScaleMat);
		m_pSprite->Begin(0);
		//m_pSprite->Draw(m_pZTexture, NULL, NULL, NULL, 0xffffffff);
		m_pSprite->End();
		pDeb->Draw();
		//終了
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}
