//========================================================================================
//
// 3Dオブジェクト
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "orbit.h"
#include "manager.h"
#include "player.h"
#include "texture.h"
//=============================================
//コンストラクタ
//=============================================

COrbit::COrbit(int nPriority) : CObject(nPriority)
{
	m_bEnd = false;
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	
	m_nLength = 0;
	m_nLife = 300;
}



//=============================================
//デストラクタ
//=============================================
COrbit::~COrbit()
{
}


//=============================================
//ポリゴン初期化関数
//=============================================
HRESULT COrbit::Init(void)
{
	m_bEnd = false;
	m_bDisp = true;
	m_nLife = 300;
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();
	
	CTexture * pTex = CManager::GetInstance()->GetTexture();
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * m_nLength,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	m_nTex = pTex->Regist("data\\TEXTURE\\OrbitBrade.png");
	VERTEX_3D * pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定

	//テクスチャ座標
	for (int nCnt = 0; nCnt < m_nLength; nCnt++)
	{
		pVtx[0].pos = GetPos();
		pVtx[1].pos = GetPos();
		pVtx[2].pos = GetPos();
		pVtx[3].pos = GetPos();

		pVtx[0].tex = D3DXVECTOR2(0.0f, (1.0f / (m_nLength)) * ((nCnt * 2)));
		pVtx[1].tex = D3DXVECTOR2(1.0f ,(1.0f / (m_nLength)) * ((nCnt * 2)));
		pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / (m_nLength)) * (((nCnt * 2)) + 1));
		pVtx[3].tex = D3DXVECTOR2(1.0f, (1.0f / (m_nLength)) * (((nCnt * 2)) + 1));



		//頂点カラーの設定
		D3DXCOLOR col = m_col;
		col.a -= (m_col.a / m_nLength) * nCnt * 2;
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		pVtx+= 4;
	}
	
	m_pPosOld = DBG_NEW D3DXVECTOR3[m_nLength * 4];
	for (int nCnt = 0; nCnt < m_nLength * 4; nCnt++)
	{
		m_pPosOld[nCnt] = GetPos();
	}
	//for (int nCnt = 0; nCnt < m_nLength * 2 + 1; nCnt += 2)
	//{

	//	pVtx[nCnt].pos = GetPos();
	//	pVtx[nCnt + 1].pos = GetPos();

	//}

	m_pVtxBuff->Unlock();
	m_bDisp = false;
	Update();
	m_bDisp = true;
	return S_OK;;
}
//=============================================
//生成関数
//=============================================
COrbit * COrbit::Create(int nLength, D3DXCOLOR col, D3DXVECTOR3 Offset1, D3DXVECTOR3 Offset2, D3DXMATRIX * pMtx, int nPriority)
{
	COrbit * pOrbit = NULL;

	pOrbit = DBG_NEW COrbit(nPriority);

	pOrbit->m_nLength = nLength;
	pOrbit->m_col = col;
	pOrbit->m_Offset1 = Offset1;
	pOrbit->m_Offset2 = Offset2;
	pOrbit->m_pMtx = pMtx;
	pOrbit->Init();
	return pOrbit;
}



//=============================================
//ポリゴンの終了処理
//=============================================
void COrbit::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	if (m_pPosOld != NULL)
	{
		delete[] m_pPosOld;
		m_pPosOld = NULL;
	}
}

//=============================================
//ポリゴンの更新処理
//=============================================
void COrbit::Update(void)
{
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	VERTEX_3D * pVtx;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX MtxOffset1, MtxOffset2;



	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&MtxOffset1);
	D3DXMatrixIdentity(&MtxOffset2);

	D3DXMatrixTranslation(&MtxOffset1, m_Offset1.x, m_Offset1.y, m_Offset1.z);
	D3DXMatrixTranslation(&MtxOffset2, m_Offset2.x, m_Offset2.y, m_Offset2.z);
	D3DXMatrixMultiply(&MtxOffset1, &MtxOffset1, m_pMtx);
	D3DXMatrixMultiply(&MtxOffset2, &MtxOffset2, m_pMtx);
	D3DXVECTOR3 Pos1, Pos2;
	Pos1 = D3DXVECTOR3(MtxOffset1._41, MtxOffset1._42, MtxOffset1._43);
	Pos2 = D3DXVECTOR3(MtxOffset2._41, MtxOffset2._42, MtxOffset2._43);
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	if (m_bDisp == true)
	{
	
		for (int nCnt = 0; nCnt < m_nLength * 2 + 1; nCnt += 2)
		{
			if (nCnt == 0)
			{
				if (!m_bEnd)
				{
					pVtx[nCnt].pos = Pos1;
					pVtx[nCnt + 1].pos = Pos2;
				}
				else
				{
					pVtx[nCnt].pos = m_pPosOld[nCnt];
					pVtx[nCnt + 1].pos = m_pPosOld[nCnt + 1];
				}
			}
			else
			{
				pVtx[nCnt].pos = m_pPosOld[nCnt - 2] ;
				pVtx[nCnt + 1].pos = m_pPosOld[nCnt - 1] ;
			}
		}
	
	}
	else
	{
		for (int nCnt = 0; nCnt < m_nLength * 2 + 1; nCnt += 2)
		{
			
				pVtx[nCnt].pos = Pos1;
				pVtx[nCnt + 1].pos = Pos2;
		
		}
	}
	for (int nCnt = 0; nCnt < m_nLength * 2 + 1; nCnt += 2)
	{
		m_pPosOld[nCnt] = pVtx[nCnt].pos;
		m_pPosOld[nCnt + 1] = pVtx[nCnt + 1].pos;
	}
		m_pVtxBuff->Unlock();
		if (m_bEnd == true)
		{
			m_nLife--;
			if (m_nLife <= 0)
			{
				Uninit();
				Release();
			}
		}
}

//=============================================
//ポリゴンの描画処理
//=============================================
void COrbit::Draw(void)
{
	if (m_bDisp == true)
	{
		CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
		LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
		pDevice = pRenderer->GetDevice();
		D3DXMATRIX mtxRot, mtxTrans; //計算用マトリクス
		CTexture * pTex = CManager::GetInstance()->GetTexture();
		D3DXMatrixIdentity(&m_mtxWorld);
	
		
		//ワールドマトリクスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, pTex->Getaddress(m_nTex));
		//レンダーステートの設定
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);//カリングの設定
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		//aブレンディングを加算合成に設定

		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nLength * 2);

		//aブレンディングを通常に設定
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//レンダーステートの設定
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//カリングの設定
	}
}

