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
#include "player.h"
#include "animbillboard.h"
#include "sound.h"
#include "player.h"
#include "camera.h"

Clist<CEnemy *> CEnemy::EnemyList = {};
//=============================================
//コンストラクタ
//=============================================
CEnemy::CEnemy() :CObject()
{
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pMotion = NULL;
	m_nLife = 0;
	m_pCollision = NULL;
	m_bShadow = true;
	for (int i = 0; i < NUM_MODEL; i++)
	{
		m_apModel[i] = NULL;
	}
	EnemyList.Regist(this);
}
//=============================================
//デストラクタ
//=============================================
CEnemy::~CEnemy()
{
	EnemyList.Delete(this);
}

//=============================================
//初期化関数
//=============================================
HRESULT CEnemy::Init()
{
	m_nStateCount = 0;
	m_nArmor = m_nLife * 0.5f;
	
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
	if (m_pCollision != NULL)
	{
		delete m_pCollision;
		m_pCollision = NULL;
	}
}
//=============================================
//更新関数
//=============================================
void CEnemy::Update()
{
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
	SetPos(GetPos() + GetMove());
	SetMove(GetMove()*0.9f);
	m_nStateCount--;
	D3DXVECTOR3 pos = GetPos();
	if (m_state == STATE_DEAD && m_nStateCount % 4 == 0)
	{
		float fSize = (rand() % 1000) * 0.1f;
	
		pos.x += (rand() % 500 - 250) *0.1f;
		pos.z += (rand() % 500 - 250) *0.1f;
		pos.y += (rand() % 1000) *0.1f;
		CAnimBillboard::Create(fSize, fSize, 6, 6, 36, 24, false, pos, "data\\TEXTURE\\spelhit.png");
	}
	if (m_nStateCount <= 0)
	{
	
		switch (m_state)
		{
		case CEnemy::STATE_DAMAGE:
			m_state = STATE_NONE;
			break;
		case CEnemy::STATE_DEAD:
			pos = GetPos();
			pos.y += 50.0f;
 			CAnimBillboard::Create(200.0f, 200.0f, 6, 6, 36, 24, false, pos, "data\\TEXTURE\\spelhit.png");
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
			pCamera->SetShake(5);
			Release();
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
		if (m_nArmor > 0)
		{
			m_nLife -= nDamage * 0.5f;
			SetMove(GetMove() + knockback * 0.5f);
			m_nArmor -= nDamage;
		}
		else
		{
			m_nLife -= nDamage;
			SetMove(GetMove() + knockback);
		}
	
		if (m_nLife <= 0)
		{
			SetState(STATE_DEAD, 120);
		}
		else
		{
			SetState(STATE_DAMAGE, 15);
		}
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
	

}
//=============================================
//更新関数
//=============================================
void CEnemy_TEST::Update()
{
	CEnemy::Update();
	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
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
			move.y -= GRAVITY;
		}
		SetMove(move);
	}
	if (m_state != STATE_DAMAGE && m_state != STATE_DEAD)
	{
		D3DXVECTOR3 vec = pPlayer->GetPos() - GetPos();
		vec.y = 0.0f;
		D3DXVec3Normalize(&vec, &vec);
		SetMove(GetMove() + vec * 0.03f);		
	}
	SetPos(GetPos() + GetMove());
	D3DXVECTOR3 move = GetMove();
	move.x *= 0.9f;
	move.z *= 0.9f;
	SetMove(move);
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


CEnemy_Walker::CEnemy_Walker() :CEnemy()
{

}

CEnemy_Walker::~CEnemy_Walker()
{
}

//=============================================
//初期化関数
//=============================================
HRESULT CEnemy_Walker::Init()
{
	CEnemy::Init();
	m_pMotion->Load("data\\TEXT\\motion_walker.txt");
	m_pMotion->SetType(0);
	m_Move = MOVE_NONE;
	m_pCollision = CSphereCollision::Create(CSphereCollision::TYPE_ENEMY, 30.0f, 0, D3DXVECTOR3(0.0f, 30.0f, -30.0f), VECTO3ZERO, &m_mtx, this);

	m_posDest.x = GetPos().x;
	m_posDest.z = GetPos().z;
	m_posDest.y = GetPos().y;
	m_rotDest = (D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CEnemy_Walker::Uninit()
{

	CEnemy::Uninit();

}
//=============================================
//更新関数
//=============================================
void CEnemy_Walker::Update()
{

	CEnemy::Update();
	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
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
			move.y -= GRAVITY;
		}
		SetMove(move);
	}
	if (m_state != STATE_DAMAGE && m_state != STATE_DEAD)
	{
		D3DXVECTOR3 vec = pPlayer->GetPos() - GetPos();
		vec.y = 0.0f;
		D3DXVec3Normalize(&vec, &vec);
		SetMove(GetMove() + vec * 0.2f);
		m_rotDest.y = atan2f(-vec.x, -vec.z);
		SetRot(GetRot() + (m_rotDest - GetRot()) * 0.1f);
		m_pMotion->SetType(MOVE_WALK);
	}
	else
	{
		m_pMotion->SetType(MOVE_DAMAGE);
	}
	SetPos(GetPos() + GetMove());
	D3DXVECTOR3 move = GetMove();
	move.x *= 0.9f;
	move.z *= 0.9f;
	SetMove(move);
	
	m_pMotion->Update();

	CEnemy::Update();
}

//=============================================
//描画関数
//=============================================
void CEnemy_Walker::Draw()
{
	CEnemy::Draw();
}
//=============================================
//生成関数
//=============================================
CEnemy_Walker * CEnemy_Walker::Create(D3DXVECTOR3 pos, int nLife)
{

	CEnemy_Walker * pEnemy = NULL;
	pEnemy = DBG_NEW  CEnemy_Walker;

	pEnemy->SetPos(pos);
	pEnemy->m_nLife = nLife;
	pEnemy->Init();

	return pEnemy;
}

CEnemy_army::CEnemy_army() :CEnemy()
{

}

CEnemy_army::~CEnemy_army()
{
}

//=============================================
//初期化関数
//=============================================
HRESULT CEnemy_army::Init()
{
	CEnemy::Init();
	m_pMotion->Load("data\\TEXT\\motion_army.txt");
	m_pMotion->SetType(0);
	m_pOrbit = NULL;
	m_pCollision = CSphereCollision::Create(CSphereCollision::TYPE_ENEMY, 30.0f, 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), VECTO3ZERO, m_apModel[1]->GetMatrixAddress(), this);
	
	m_posDest.x = GetPos().x;
	m_posDest.z = GetPos().z;
	m_posDest.y = GetPos().y;
	m_rotDest = (D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));

	m_nRoutineCount = 0;
	m_Routine = ROUTINE_WAIT;
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CEnemy_army::Uninit()
{

	CEnemy::Uninit();

}
//=============================================
//更新関数
//=============================================
void CEnemy_army::Update()
{

	CEnemy::Update();
	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
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
				move.y -= GRAVITY;
		}
		SetMove(move);
	}
	if (m_state != STATE_DAMAGE && m_state != STATE_DEAD && m_pMotion->GetType() != MOTION_DAMAGE)
	{
		D3DXVECTOR3 vec = pPlayer->GetPos() - GetPos();
		switch (m_Routine)
		{
		case CEnemy_army::ROUTINE_WAIT:
			m_pMotion->SetType(MOTION_NONE);
			break;
		case CEnemy_army::ROUTINE_FORWARD:
			vec.y = 0.0f;
			D3DXVec3Normalize(&vec, &vec);
			SetMove(GetMove() + vec * 0.2f);
			m_rotDest.y = atan2f(-vec.x, -vec.z);
			SetRot(GetRot() + (m_rotDest - GetRot()) * 0.15f);
			m_pMotion->SetType(MOTION_WALK);
			break;
		case CEnemy_army::ROUTINE_BACK:
			vec.y = 0.0f;
			D3DXVec3Normalize(&vec, &vec);
			SetMove(GetMove() + vec * -0.2f);
			m_rotDest.y = atan2f(-vec.x, -vec.z);
			SetRot(GetRot() + (m_rotDest - GetRot()) * 0.15f);
			m_pMotion->SetType(MOTION_WALK);
			break;
		case CEnemy_army::ROUTINE_ATTACK:
			if (m_pOrbit == NULL)
			{
				m_pOrbit = COrbit::Create(60, D3DXCOLOR(1.0f, 0.25f, 0.5f, 1.0f), D3DXVECTOR3(-1.5f, 10.0f, -75.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress(), "data\\TEXTURE\\OrbitBrade.png");
			}
		
			vec.y = 0.0f;
			D3DXVec3Normalize(&vec, &vec);
			
			m_rotDest.y = atan2f(-vec.x, -vec.z);
			SetRot(GetRot() + (m_rotDest - GetRot()) * 0.15f);
			m_pMotion->SetType(MOTION_ATTACK);
			break;
		case CEnemy_army::ROUTINE_MAX:
			break;
		default:
			break;
		}


	}

	SetPos(GetPos() + GetMove());
	D3DXVECTOR3 move = GetMove();
	if (m_pMotion->GetType() != MOTION_DAMAGE)
	{
		move.x *= 0.9f;
		move.z *= 0.9f;
	}
	
	SetMove(move);

	m_pMotion->Update();
	m_nRoutineCount--;
	if (m_nRoutineCount <= 0 && m_pMotion->GetType() != MOTION_ATTACK)
	{
		SetRoutine((ROUTINE)(rand() % ROUTINE_MAX), rand()%120+ 30);
		if (m_pOrbit != NULL)
		{
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}
	CEnemy::Update();
}

//=============================================
//描画関数
//=============================================
void CEnemy_army::Draw()
{
	CEnemy::Draw();
}
bool CEnemy_army::Damage(int nDamage, D3DXVECTOR3 knockback)
{
	
	if (CEnemy::Damage(nDamage, knockback))
	{
		if (m_nArmor <= 0)
		{
			m_rotDest.y = atan2f(-GetMove().x, -GetMove().z);
			m_pMotion->SetType(MOTION_DAMAGE);
			m_pMotion->SetkeyNumber(0);
		}
		
		return true;
	}
	return false;

}
//=============================================
//生成関数
//=============================================
CEnemy_army * CEnemy_army::Create(D3DXVECTOR3 pos, int nLife)
{

	CEnemy_army * pEnemy = NULL;
	pEnemy = DBG_NEW  CEnemy_army;

	pEnemy->SetPos(pos);
	pEnemy->m_nLife = nLife;
	pEnemy->Init();

	return pEnemy;
}