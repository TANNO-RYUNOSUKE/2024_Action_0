//========================================================================================
//
// ゲージ
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード

#include "object2D.h"
#include "scene.h"
#include "Enemy.h"
#include "manager.h"

#include "lifegage.h"
#include "player.h"
#include "scene.h"
//=============================================
//コンストラクタ
//=============================================
CGage::CGage()
{
	m_pGage = NULL;
	m_pGageRed = NULL;
}



//=============================================
//デストラクタ
//=============================================
CGage::~CGage()
{
}


//=============================================
//ポリゴン初期化関数
//=============================================
HRESULT CGage::Init(void)
{
	m_ndata = 0;
	
	
	m_pGageRed = CObject2D::Create(GetPos(), 10.0f, m_fWidth, 0, NULL, D3DXVECTOR2(0.0f, 0.5f));
	m_pGage = CObject2D::Create(GetPos(), 10.0f, m_fWidth, 0, "data\\TEXTURE\\life_gage.png", D3DXVECTOR2(0.0f, 0.5f));

	m_pGageRed->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	return S_OK;
}
//=============================================
//生成関数
//=============================================
CGage * CGage::Create(D3DXVECTOR3 pos, float fWidth, int nMax)
{
	CGage * pEnemy_UI = NULL;
	pEnemy_UI = DBG_NEW CGage;
	pEnemy_UI->m_fWidth = fWidth;
	pEnemy_UI->SetPos(pos);
	pEnemy_UI->m_nMax = nMax;
	pEnemy_UI->Init();
	return pEnemy_UI;
}



//=============================================
//ポリゴンの終了処理
//=============================================
void CGage::Uninit(void)
{

}

//=============================================
//ポリゴンの更新処理
//=============================================
void CGage::Update(void)
{
	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	m_pGage->SetTexMax(D3DXVECTOR2(m_pGage->GetTexMax().x - 0.01f, m_pGage->GetTexMax().y));
	m_pGage->SetTexMin(D3DXVECTOR2(m_pGage->GetTexMin().x - 0.01f, m_pGage->GetTexMin().y));
	if (pPlayer != NULL)
	{
		m_ndata = pPlayer->GetLife();
		
		m_pGage->SetWidth(m_pGage->GetWidth() + (((m_fWidth/(float)m_nMax)* pPlayer->GetLife()) - m_pGage->GetWidth()) * 0.3f);
		m_pGageRed->SetWidth(m_pGageRed->GetWidth() + (((m_fWidth / (float)m_nMax)* pPlayer->GetLife()) - m_pGageRed->GetWidth()) * 0.05f);
	}
	else
	{
		m_ndata = 0;
		m_pGage->SetWidth(m_pGage->GetWidth() - m_pGage->GetWidth() * 0.3f);
		m_pGageRed->SetWidth(m_pGageRed->GetWidth() - m_pGageRed->GetWidth() * 0.05f);
	}

}

//=============================================
//ポリゴンの描画処理
//=============================================
void CGage::Draw(void)
{
	
}
