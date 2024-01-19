//========================================================================================
//
// フェード
// Author: 丹野 竜之介
//
//========================================================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"

CFade::CFade() :CObject2D(7)
{
	m_State = STATE_NONE;
}

CFade::~CFade()
{
}
//=============================================
//初期化関数
//=============================================
HRESULT CFade::Init()
{
	m_State = STATE_FADEIN;
	SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	SetHeight(SCREEN_HEIGHT);
	SetWidth(SCREEN_WIDTH);
	CObject2D::Init();
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CFade::Uninit()
{
	CObject2D::Uninit();
}
//=============================================
//更新関数
//=============================================
void CFade::Update()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	VERTEX_2D * pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	D3DXCOLOR col = GetCol();
	if (m_State != STATE_NONE)
	{
		if (m_State == STATE_FADEIN)
		{
			col.a -= 0.01f;
			if (col.a <= 0.0f)
			{
				col.a = 0.0f;
				m_State = STATE_NONE;
			}
		}
		if (m_State == STATE_FADEOUT)
		{
			col.a += 0.01f;
			if (col.a >= 1.0f)
			{
				col.a = 1.0f;
			
			}
		}
		SetCol(col);
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

	}
	m_pVtxBuff->Unlock();
	if (m_State == STATE_FADEOUT)
	{
		if (col.a >= 1.0f)
		{
			m_State = STATE_NONE;
			CManager::GetInstance()->SetMode(m_modenext);
		}
	}
}
//=============================================
//描画関数
//=============================================
void CFade::Draw()
{
	if (m_State != STATE_NONE)
	{
		CObject2D::Draw();
	}
}