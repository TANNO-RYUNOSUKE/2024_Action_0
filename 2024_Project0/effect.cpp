//========================================================================================
//
// エフェクト
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "object2D.h"
#include "effect.h"
#include "input.h"

#include "enemy.h"
#include "sound.h"
#include "texture.h"


LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;
//=============================================
//コンストラクタ
//=============================================
CEffect::CEffect(int nPriority) : CBillboard(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_fSize = 0;
	Rot = 0.0f;
	Rotspeed = 0.0f;
}
//=============================================
//デストラクタ
//=============================================
CEffect::~CEffect()
{
}

//=============================================
//初期化関数
//=============================================
HRESULT CEffect::Init(char * pName)
{
	CTexture * pTex = CManager::GetInstance()->GetTexture();

 	m_nIdxTex[0] = pTex->Regist(pName);
	CBillboard::Init();
	

	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CEffect::Uninit()
{
	
	CBillboard::Uninit();
}
//=============================================
//更新関数
//=============================================
void CEffect::Update()
{
	VERTEX_3D * pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;
	
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-GetWidth() , +GetHeight() , 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+GetWidth() , +GetHeight() , 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetWidth() , -GetHeight() , 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+GetWidth() , -GetHeight() , 0.0f);
	D3DXMATRIX mtx;
	;
	D3DXMatrixRotationZ(&mtx, Rot);
	D3DXVec3TransformCoord(&pVtx[0].pos, &pVtx[0].pos, &mtx);
	D3DXVec3TransformCoord(&pVtx[1].pos, &pVtx[1].pos, &mtx);
	D3DXVec3TransformCoord(&pVtx[2].pos, &pVtx[2].pos, &mtx);
	D3DXVec3TransformCoord(&pVtx[3].pos, &pVtx[3].pos, &mtx);
	Rot += Rotspeed;
	m_pVtxBuff->Unlock();
	SetPos(GetPos() + m_move);
	m_move *= m_fAttenuation;
	SetWidth(GetWidth()  *m_fAttenuation);
	SetHeight(GetHeight() *m_fAttenuation);
	m_Color.a = (float)m_nLife / (float)m_nMaxLife;
	m_nLife--;

	if (m_nLife <= 0)
	{
		CObject::Release();
	}


}
//=============================================
//描画関数
//=============================================
void CEffect::Draw()
{
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();
	//アルファテストを有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//aブレンディングを加算合成に設定

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	CBillboard::Draw();

	//アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 80);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//aブレンディングを通常に設定

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
}
//=============================================
//生成関数
//=============================================
CEffect * CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, float fsize, float fAttenuation,float rot, float rotspeed ,  char * pName)
{
	CEffect * pEffect = NULL;
	pEffect = DBG_NEW CEffect;

	pEffect->m_Color = col;
	pEffect->SetPos(pos);
	pEffect->SetWidth(fsize);
	pEffect->SetHeight(fsize);
	pEffect->m_move = move;
	pEffect->m_nLife = nLife;
	pEffect->m_nMaxLife = nLife;
	pEffect->m_fAttenuation = fAttenuation;
	pEffect->Rot = rot;
	pEffect->Rotspeed = rotspeed;
	pEffect->Init(pName);
	return pEffect;
}

