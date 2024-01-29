//========================================================================================
//
// バレット
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "object2D.h"
#include "bullet.h"
#include "input.h"

#include "enemy.h"
#include "sound.h"

#include "camera.h"
#include "texture.h"
#include "scene.h"
#include "player.h"
#include "meshfield.h"
#include "objectX.h"
#include "Xmanager.h"
#include "scene.h"



//=============================================
//コンストラクタ
//=============================================
CBullet::CBullet(int nPriority):CObject(nPriority)
{
	m_nLife = 0;

	m_Type = TYPE_NONE;

}
//=============================================
//デストラクタ
//=============================================
CBullet::~CBullet()
{
}

//=============================================
//初期化関数
//=============================================
HRESULT CBullet::Init()
{
	Update();
	switch (m_Type)
	{
	case CBullet::TYPE_NONE:
		break;
	case CBullet::TYPE_PLAYER:
		m_pColl = CSphereCollision::Create(CSphereCollision::TYPE_AUDIENCEATTACK, 10.0f, 5, VECTO3ZERO, VECTO3ZERO,GetMatrixAddress(),this);
		m_pOrbit = COrbit::Create(60, D3DXCOLOR(0.7f, 1.0f, 0.3f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), GetMatrixAddress());
		break;
	case CBullet::TYPE_ENEMY:
		break;
	case CBullet::TYPE_MAX:
		break;
	default:
		break;
	}
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CBullet::Uninit()
{
	if (m_pOrbit != NULL)
	{
		m_pOrbit->end();
		m_pOrbit = nullptr;
	}
	if (m_pColl != NULL)
	{
		delete m_pColl;
		m_pColl = NULL;
	}
}
//=============================================
//更新関数
//=============================================
void CBullet::Update()
{

	m_nLife--;
	SetPos(GetPos() + GetMove());
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリクス

	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 pos = GetPos();
	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtx);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxTrans);
	if (m_nLife <= 0)
	{
		CObject::Release();
	}
	
}
//=============================================
//描画関数
//=============================================
void CBullet::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();


}
//=============================================
//生成関数
//=============================================
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, TYPE type)
{
	CBullet * pBullet = NULL;
	pBullet = DBG_NEW  CBullet;
	
	pBullet->SetPos(pos);
	
	pBullet->SetMove(move);
	
	pBullet->m_Type = type;
	pBullet->m_nLife = nLife;

	pBullet->Init();
	return pBullet;
}
