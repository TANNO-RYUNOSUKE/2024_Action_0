//========================================================================================
//
// 2Dオブジェクト
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "object2D.h"
#include "manager.h"
#include "texture.h"

//=============================================
//コンストラクタ
//=============================================

CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fHeight = 0.0f;
	m_nLife = -1000;
	m_fWidth = 0.0f;
	m_pFileName = NULL;
}



//=============================================
//デストラクタ
//=============================================
CObject2D::~CObject2D()
{
}


//=============================================
//ポリゴン初期化関数
//=============================================
HRESULT CObject2D::Init(void)
{
	m_bDisp = true;
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	if (m_pVtxBuff == NULL)
	{
		return E_FAIL;
	}
	CTexture * pTex = CManager::GetInstance()->GetTexture();

	if (m_pFileName != NULL)
	{
	
		m_nIdxTex[0] = pTex->Regist(m_pFileName);

	}

	VERTEX_2D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//頂点座標の設定
	SetVtx(pVtx);

	//法線の設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	m_pVtxBuff->Unlock();

	return S_OK;;
}
//=============================================
//生成関数
//=============================================
CObject2D * CObject2D::Create(D3DXVECTOR3 pos, float fHeight, float fWidth, int nPriority,char * pFileName)
{
	CObject2D * pObject2D = NULL;
	pObject2D = DBG_NEW  CObject2D(nPriority);
	pObject2D->SetPos(pos);
	pObject2D->SetHeight(fHeight);
	pObject2D->SetWidth(fWidth);
	pObject2D->m_pFileName = pFileName;
	pObject2D->Init();
	return pObject2D;
}



//=============================================
//ポリゴンの終了処理
//=============================================
void CObject2D::Uninit(void)
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
void CObject2D::Update(void)
{
	if (m_nLife >0)
	{
		m_nLife--;
		if (m_nLife <= 0)
		{
			Release();
			return;
		}
	}
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();

	VERTEX_2D *pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	//頂点座標の設定
	SetVtx(pVtx);

	
	m_pVtxBuff->Unlock();
	SetPos(GetPos() + GetMove());
}

//=============================================
//ポリゴンの描画処理
//=============================================
void CObject2D::Draw(void)
{
	if (m_bDisp == true)
	{
		CTexture * pTex = CManager::GetInstance()->GetTexture();
		CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
		LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
		pDevice = pRenderer->GetDevice();

		//aブレンディングを通常に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, pTex->Getaddress(m_nIdxTex[0]));

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	}
}
//=============================================
//設定
//=============================================
 void CObject2D::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth)
{
	SetPos(pos);
	SetRot(rot);
	SetHeight(fHeight);
	SetWidth(fWidth);
}
 //=============================================
 //頂点の最適化
 //=============================================
 void CObject2D::SetVtx(VERTEX_2D  * pVtx)
 {
	 D3DXVECTOR3 pos = GetPos();
	 D3DXVECTOR3 rot = GetRot();
	 float fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) * 0.5f;
	 float fAngle = atan2f(m_fWidth, m_fHeight);

	 pVtx[0].pos.x = pos.x + sinf(rot.z - D3DX_PI + fAngle)*fLength;
	 pVtx[0].pos.y = pos.y + cosf(rot.z - D3DX_PI + fAngle)*fLength;
	 pVtx[0].pos.z = 0.0f;

	 pVtx[1].pos.x = pos.x + sinf(rot.z + D3DX_PI - fAngle)*fLength;
	 pVtx[1].pos.y = pos.y + cosf(rot.z + D3DX_PI - fAngle)*fLength;
	 pVtx[1].pos.z = 0.0f;

	 pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle)*fLength;
	 pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle)*fLength;
	 pVtx[2].pos.z = 0.0f;

	 pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle)*fLength;
	 pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle)*fLength;
	 pVtx[3].pos.z = 0.0f;

	 //頂点カラーの設定
	 pVtx[0].col = m_col;
	 pVtx[1].col = m_col;
	 pVtx[2].col = m_col;
	 pVtx[3].col = m_col;
 }															