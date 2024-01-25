//========================================================================================
//
// エネミー
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "list.h"
#include "scene.h"
#include "meshfield.h"
//=============================================
//コンストラクタ
//=============================================
CEnemy::CEnemy() :CObject()
{
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pMotion = NULL;
	m_nLife = 0;
	for (int i = 0; i < NUM_MODEL; i++)
	{
		m_apModel[i] = NULL;
	}
	

}
//=============================================
//デストラクタ
//=============================================
CEnemy::~CEnemy()
{
}

//=============================================
//初期化関数
//=============================================
HRESULT CEnemy::Init()
{
	m_nStateCount = 0;

	
	m_pMotion = DBG_NEW  CMotion;
	m_pMotion->SetModel(&m_apModel[0]);
	SetType(CObject::TYPE_ENEMY);

	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CEnemy::Uninit()
{
	
	if (m_pMotion != NULL)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}
	for (int nCnt = 0; nCnt < NUM_MODEL; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{
			m_apModel[nCnt]->Uninit();
			delete m_apModel[nCnt]; // ポインタを解放
			m_apModel[nCnt] = NULL;
		}

	}

}
//=============================================
//更新関数
//=============================================
void CEnemy::Update()
{
	SetPos(GetPos() + GetMove());
	SetMove(GetMove()*0.9f);
	m_nStateCount--;
	if (m_nStateCount <= 0)
	{
		switch (m_state)
		{
		case CEnemy::STATE_DAMAGE:
			m_state = STATE_NONE;
			break;
		case CEnemy::STATE_DEAD:
			break;
		default:
			m_nStateCount = 0;
			break;
		}
	}
}

//=============================================
//描画関数
//=============================================
void CEnemy::Draw()
{
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();
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

	//ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtx);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCnt = 0; nCnt < NUM_MODEL; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{
			m_apModel[nCnt]->Draw();
		}
	}

}
//=============================================
//生成関数
//=============================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, int nLife)
{

	CEnemy * pEnemy = NULL;
	pEnemy = DBG_NEW  CEnemy;
	pEnemy->SetPos(pos);
	pEnemy->m_nLife = nLife;
	pEnemy->Init();
	return pEnemy;
}
bool CEnemy::Damage(int nDamage, D3DXVECTOR3 knockback)
{
	if (m_state != STATE_DAMAGE && m_state != STATE_DEAD)
	{
		m_nLife -= nDamage;
		SetMove(GetMove() + knockback);
		SetState(STATE_DAMAGE, 15);
		return true;
	}
	return false;
}
//=============================================
//コンストラクタ
//=============================================
CEnemy_TEST::CEnemy_TEST()
{
	m_pCollision = NULL;
}
//=============================================
//デストラクタ
//=============================================
CEnemy_TEST::~CEnemy_TEST()
{
}

//=============================================
//初期化関数
//=============================================
HRESULT CEnemy_TEST::Init()
{
	CEnemy::Init();
	m_pCollision = CSphereCollision::Create(CSphereCollision::TYPE_ENEMY, 30.0f, 0, D3DXVECTOR3(0.0f, 30.0f, 0.0f), VECTO3ZERO, &m_mtx, this);


	auto t = CSphereCollision::List.GetList()->begin();
	m_apModel[0] = CModel::Create("data\\MODEL\\testBlock.x");
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CEnemy_TEST::Uninit()
{
	CEnemy::Uninit();
	if (m_pCollision != NULL)
	{
		delete m_pCollision;
		m_pCollision = NULL;
	}

}
//=============================================
//更新関数
//=============================================
void CEnemy_TEST::Update()
{
	CEnemy::Update();
	CMeshfield * pMesh = CManager::GetInstance()->GetScene()->GetMeshfield();
	if (pMesh != NULL)
	{
		D3DXVECTOR3 move = GetMove();
		if (pMesh->Collision(GetPosAddress()))
		{
			move.y = 0.0f;
		}
		else
		{
			move.y -= 0.4f;
		}
		SetMove(move);
	}
}

//=============================================
//描画関数
//=============================================
void CEnemy_TEST::Draw()
{
	CEnemy::Draw();

}
//=============================================
//生成関数
//=============================================
CEnemy_TEST * CEnemy_TEST::Create(D3DXVECTOR3 pos, int nLife)
{

	CEnemy_TEST * pEnemy = NULL;
	pEnemy = DBG_NEW  CEnemy_TEST;
	pEnemy->SetPos(pos);
	pEnemy->m_nLife = nLife;
	pEnemy->Init();
	return pEnemy;
}