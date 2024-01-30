//============================================================
//
//	攻撃ビット管理 [audience.cpp]
//	Author：丹野竜之介
//
//============================================================
#include "Audience.h"
#include "renderer.h"
#include "manager.h"
#include "enemy.h"
#include "player.h"
#include "manager.h"
#include "scene.h"
#include "Supporter.h"
#include "bullet.h"

Clist<CAudience *> CAudience::List = {};

//====================================
//コンストラクタ、デストラクタ
//====================================
CAudience::CAudience(int nPriority) : CObject(nPriority)
{
	m_nStateCount = 0;
	m_RotDest = VECTO3ZERO;
	m_PosDest = VECTO3ZERO;
	m_state = STATE_NONE;
	m_pModel = NULL;
	List.Regist(this);
}

CAudience::~CAudience()
{
	List.Delete(this);
}

//====================================
//初期化
//====================================
HRESULT CAudience::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	m_pModel = CObjectX::Create("data\\MODEL\\Audience.x",VECTO3ZERO);
	m_state = STATE_NEUTRAL;
	m_pOrbit = COrbit::Create(60, D3DXCOLOR(0.7f, 1.0f, 0.3f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), GetMatrixAddress());
	m_pOrbit2 = COrbit::Create(60, D3DXCOLOR(0.7f, 1.0f, 0.3f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), GetMatrixAddress());
	return S_OK;
}
//====================================
//終了
//====================================
void CAudience::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		m_pModel = NULL;
	}
	if (m_pOrbit != NULL)
	{
		m_pOrbit->Uninit();
		m_pOrbit = NULL;
	}
	if (m_pOrbit2 != NULL)
	{
		m_pOrbit2->Uninit();
		m_pOrbit2 = NULL;
	}
}
//====================================
//更新
//====================================
void CAudience::Update(void)
{
	D3DXVECTOR3 animRot = m_RotDest - GetRot();
	if (animRot.x< -D3DX_PI)
	{
		animRot.x += D3DX_PI * 2;
	}
	else if (animRot.x > D3DX_PI)
	{
		animRot.x += -D3DX_PI * 2;
	}
	if (animRot.y< -D3DX_PI)
	{
		animRot.y += D3DX_PI * 2;
	}
	else if (animRot.y > D3DX_PI)
	{
		animRot.y += -D3DX_PI * 2;
	}
	if (animRot.z< -D3DX_PI)
	{
		animRot.z += D3DX_PI * 2;
	}
	else if (animRot.z > D3DX_PI)
	{
		animRot.z += -D3DX_PI * 2;
	}
	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	if (pPlayer != NULL)
	{
		m_PosDest = pPlayer->GetPos();
		m_PosDest.y += 100.0f;
		m_RotDest = VectorToAngles(GetPos()- m_PosDest);
	}
	D3DXVECTOR3 vec = m_PosDest - GetPos();
	D3DXVec3Normalize(&vec, &vec);
	SetMove(GetMove() + vec* 0.03f);
	SetPos(GetPos() + GetMove());
	SetMove(GetMove()* 0.99f);
	SetRot(GetRot() + animRot /30);
	m_pModel->SetPos(GetPos());
	m_pModel->SetRot(GetRot());
	Attack();
}
//====================================
//描画
//====================================
void CAudience::Draw(void)
{	
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

	m_pModel->Draw();
}
//====================================
//攻撃
//====================================
void CAudience::Attack()
{
	m_nStateCount--;
	switch (m_state)
	{
	case CAudience::STATE_NONE:
		break;
	case CAudience::STATE_NEUTRAL:
		break;
	case CAudience::STATE_AUTOATTACK:
		if (m_nStateCount % 60 == 0)
		{
			CEnemy * pEnemy = NULL;
			int nNum = CEnemy::EnemyList.GetNum();
			if (nNum > 0)
			{
				int nTarget = rand() % nNum;
				pEnemy = CEnemy::EnemyList.Get(nTarget);
			}
			if (pEnemy != NULL)
			{
				D3DXVECTOR3 Vec = pEnemy->GetPos() - GetPos();
				D3DXVec3Normalize(&Vec, &Vec);
				CBullet::Create(GetPos(), Vec * 10.0f, 60, CBullet::TYPE_PLAYER);
			}
		}
		break;
	case CAudience::STATE_MANUALATTACK:
		if (m_nStateCount % 6 == 0)
		{
			CEnemy * pEnemy = NULL;
			int nNum = CEnemy::EnemyList.GetNum();
			if (nNum > 0)
			{
				int nTarget = rand() % nNum;
				pEnemy = CEnemy::EnemyList.Get(nTarget);
			}
			if (pEnemy != NULL)
			{
				CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
				if (pPlayer != NULL)
				{
					if (pPlayer->GetTarget() != NULL)
					{
						D3DXVECTOR3 Vec = { (rand() % 1000 - 500) *0.1f,(rand() % 1000 - 500) *0.1f,(rand() % 1000 - 500) *0.1f };
						D3DXVec3Normalize(&Vec, &Vec);
						CMissile::Create(GetPos(), Vec, 60, CBullet::TYPE_PLAYER, 10.0f, 0.1f, pPlayer->GetTarget());
					}
					else
					{
						D3DXVECTOR3 Vec = pEnemy->GetPos() - GetPos();
						D3DXVec3Normalize(&Vec, &Vec);
						CBullet::Create(GetPos(), Vec * 20.0f, 60, CBullet::TYPE_PLAYER);
					}
				}
			
			}
		}
		break;
	case CAudience::STATE_DEATH:
		break;
	case CAudience::STATE_MAX:
		break;
	default:
		break;
	}
	if (m_nStateCount <= 0)
	{
		m_nStateCount = 0;
		switch (m_state)
		{
		case CAudience::STATE_NONE:
			break;
		case CAudience::STATE_NEUTRAL:
			SetState(STATE_AUTOATTACK, 300);
			break;
		case CAudience::STATE_AUTOATTACK:
			SetState(STATE_NEUTRAL, 60);
			break;
		case CAudience::STATE_MANUALATTACK:
			SetState(STATE_NEUTRAL, 60);
			break;
		case CAudience::STATE_DEATH:
			break;
		case CAudience::STATE_MAX:
			break;
		default:
			break;
		}
	}
	
}
//====================================
//生成
//====================================
CAudience * CAudience::Create(void)
{
	CAudience * pAudience = DBG_NEW CAudience;
	pAudience->Init();
	return pAudience;
}
void CAudience::SetStateAll(STATE state, int nCnt) 
{
	int nSize = List.GetNum(); for (int i = 0; i < nSize; i++)
	{
		List.Get(i)->SetState(state, nCnt);
	}
}