//========================================================================================
//
// アニメーションビルボード
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "object3D.h"
#include "manager.h"
#include "AnimBillboard.h"
#include "texture.h"

//=============================================
//コンストラクタ
//=============================================

CAnimBillboard::CAnimBillboard(int nPriority) : CBillboard(nPriority)
{

}



//=============================================
//デストラクタ
//=============================================
CAnimBillboard::~CAnimBillboard()
{
}


//=============================================
//ポリゴン初期化関数
//=============================================
HRESULT CAnimBillboard::Init(void)
{

	
	return S_OK;;
}
//=============================================
//生成関数
//=============================================
CAnimBillboard * CAnimBillboard::Create(float fHeight, float fWidth, int nH_Key, int nW_Key, int nNumKey, int nFlame, bool bLoop, D3DXVECTOR3 pos, char * pName )
{
	CAnimBillboard * pObject3D = NULL;

	pObject3D = DBG_NEW CAnimBillboard;
	if (pName != NULL)
	{
		CTexture * pTex = CManager::GetInstance()->GetTexture();

		pObject3D->m_nIdxTex[0] = pTex->Regist(pName);
	}

	pObject3D->SetHeight(fHeight);
	pObject3D->SetWidth(fWidth);
	pObject3D->SetPos(pos);
	pObject3D->Init();
	return pObject3D;
}



//=============================================
//ポリゴンの終了処理
//=============================================
void CAnimBillboard::Uninit(void)
{

	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================
//ポリゴンの更新処理
//=============================================
void CAnimBillboard::Update(void)
{
	SetVtx();
}

//=============================================
//ポリゴンの描画処理
//=============================================
void CAnimBillboard::Draw(void)
{


}
