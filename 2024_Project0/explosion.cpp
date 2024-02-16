//========================================================================================
//CExplosion
// フェード
// Author: 丹野 竜之介
//
//========================================================================================
#include "explosion.h"
#include "manager.h"
#include "animbillboard.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
CExplosion::CExplosion()
{

}

CExplosion::~CExplosion()
{
}
//=============================================
//初期化関数
//=============================================
HRESULT CExplosion::Init()
{
	CAnimBillboard::Create(400.0f, 400.0f, 13, 5, 65, 60, false, GetPos(), "data\\TEXTURE\\charge.jpg");
	pCol = NULL;
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CExplosion::Uninit()
{
	if (pCol != NULL)
	{
		delete pCol;
		pCol = NULL;
	}
}
//=============================================
//更新関数
//=============================================
void CExplosion::Update()
{
	m_nLife--;
	if (m_nLife == 0)
	{

		CAnimBillboard::Create(600.0f, 800.0f, 13, 5, 65, 30, false, GetPos(), "data\\TEXTURE\\explosion.jpg");
		pCol = CSphereCollision::Create(CSphereCollision::TYPE_ENEMYATTACK, 800.0f, 15, GetPos(), D3DXVECTOR3(0.0f, 50.0f, 0.0f), NULL, this);
		CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
	} 
	if (m_nLife <= -30)
	{
		Release();
	}
}
//=============================================
//描画関数
//=============================================
void CExplosion::Draw()
{

}
CExplosion * CExplosion::Create(D3DXVECTOR3 pos, int nLife)
{
	CExplosion * pEx = NULL;
	pEx = DBG_NEW CExplosion();
	pEx->SetPos(pos);
	pEx->SetLife(nLife);
	pEx->Init();
	return pEx;
}