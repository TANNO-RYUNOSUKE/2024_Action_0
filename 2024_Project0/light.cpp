//====================================
//ライト処理
//====================================
#include "light.h"
#include "renderer.h"
#include "manager.h"

//====================================
//コンストラクタ、デストラクタ
//====================================
CLight::CLight()
{
}

CLight::~CLight()
{
}

//====================================
//初期化
//====================================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir; //設定用方向ベクトル
	//ライトの情報をクリアする
	ZeroMemory(&m_aLight, sizeof(D3DLIGHT9));
	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		
		//ライトの種類の設定
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{
		case 0:
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			vecDir = D3DXVECTOR3(-1.4f, 0.24f, -1.21f);
			break;
		case 1:
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.9f, 0.6f);
			vecDir = D3DXVECTOR3(1.42f, -0.8f, 0.08f);
			break;
		case 2:
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.2f);
			vecDir = D3DXVECTOR3(-0.59f, -0.8f, 1.55f);

		default:
			break;
		}
		D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトル正規化
		m_aLight[nCntLight].Direction = vecDir;



		//ライトを設定する
		pDevice->SetLight(0, &m_aLight[nCntLight]);

	}
	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);
	return S_OK;
}
//====================================
//終了
//====================================
void CLight::Uninit(void)
{

}
//====================================
//更新
//====================================
void CLight::Update(void)
{

}