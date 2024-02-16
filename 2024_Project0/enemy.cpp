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
#include "lifegage.h"
#include "player.h"
#include "camera.h"
#include "bullet.h"
#include "Supporter.h"
#include "explosion.h"

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
	m_bZ = true;
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
	m_nArmor = m_nLife;
	m_nArmorMax = m_nArmor;
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
	pos = GetPos();
	

	if (pos.x > 1200.0f)
	{
		pos.x = 1200.0f;
	}
	else if (pos.x < -800.0f)
	{
		pos.x = -800.0f;
	}
	if (pos.z > 16000.0f)
	{
		pos.z = 16000.0f;
	}
	else if (pos.z < -5500.0f)
	{
		pos.z = -5500.0f;
	}
	SetPos(pos);
	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	if (m_state == STATE_DAMAGE && pPlayer != NULL && m_type != TYPE_BOSS)
	{
		if (pPlayer->GetTarget() == this)
		{
			SetMove(GetMove() + (pPlayer->GetAttackPos() - GetPos()) * 0.005f + pPlayer->GetMove() * 0.1f);
			D3DXVECTOR3 move = GetMove();
			move.y += GRAVITY * 0.8f - pPlayer->GetMove().y * 0.1f;
			SetMove(move);
		}
		else
		{
			SetMove(GetMove() + (pPlayer->GetAttackPos() - GetPos()) * 0.0025f);
		}
		
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
		if (m_nArmor <= 0)
		{
			m_nArmor = m_nArmorMax * 0.75f;
		}
		if (m_nArmor > m_nArmorMax * 0.5f)
		{
			m_nLife -= nDamage * 0.5f;
			SetMove(GetMove() + knockback * 0.5f);
			m_nArmor -= nDamage;
		}
		else
		{
			m_nLife -= nDamage;
			SetMove(GetMove() + knockback);
			m_nArmor -= nDamage;
		
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
	m_pAttackCollision = NULL;
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
	if (m_pAttackCollision != NULL)
	{
		delete m_pAttackCollision;
		m_pAttackCollision = NULL;
	}
	CEnemy::Uninit();

}
//=============================================
//更新関数
//=============================================
void CEnemy_army::Update()
{

	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	CMeshfield * pMesh = CManager::GetInstance()->GetScene()->GetMeshfield();

	if (m_state != STATE_DAMAGE && m_state != STATE_DEAD && m_pMotion->GetType() != MOTION_DAMAGE && m_pMotion->GetType() != MOTION_DOWN)
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
			SetRot(GetRot() + (m_rotDest - GetRot()) * 0.3f);
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
			Attack();
			break;
		case CEnemy_army::ROUTINE_MAX:
			break;
		default:
			break;
		}


	}

	
	D3DXVECTOR3 vec = pPlayer->GetPos() - GetPos();
	vec.y = 0.0f;
	m_pMotion->Update();
	m_nRoutineCount--;
	if (GetDistance(vec) < 80.0f)
	{
		m_nRoutineCount -= 2;
	}
	if (m_pAttackCollision != NULL && m_pMotion->GetType() != MOTION_ATTACK)
	{
		delete m_pAttackCollision;
		m_pAttackCollision = NULL;
	}
	if (m_nRoutineCount <= 0 && m_pMotion->GetType() != MOTION_ATTACK)
	{
		
		if (GetDistance(vec) < 80.0f)
		{
			SetRoutine((ROUTINE)(rand() % ROUTINE_MAX), 30);
		}
		else
		{
			SetRoutine((ROUTINE)(rand() % ROUTINE_ATTACK), rand() % 120 + 30);
		}
	
		if (m_pOrbit != NULL)
		{
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
	}


		D3DXVECTOR3 move = GetMove();
		move.y -= GRAVITY;
		if (GetPos().y <= 0.0f)
		{
			D3DXVECTOR3 pos = GetPos();
			pos.y = 0.0f;
			if (move.y < 0.0f)
			{
				move.y = 0.0f;
			}
		
			SetPos(pos);
		}

		SetMove(move);
	
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
		if (m_nArmor <= m_nArmorMax * 0.5f)
		{
			m_rotDest.y = atan2f(-GetMove().x, -GetMove().z);
			m_pMotion->SetType(MOTION_DAMAGE);
			m_pMotion->SetkeyNumber(0);
		}
		if (m_nArmor <= 0)
		{
			m_pMotion->SetType(MOTION_DOWN);
			m_pMotion->SetkeyNumber(0);
		}
		
		return true;
	}
	return false;

}
void  CEnemy_army::Attack()
{
	m_pMotion->SetType(MOTION_ATTACK);
	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	D3DXVECTOR3 vec = (pPlayer->GetPos() - GetPos());
	m_rotDest.y = atan2f(-vec.x, -vec.z);
	SetRot(GetRot() + (m_rotDest - GetRot()) * 0.15f);
	if (m_pMotion->GetKey() == 2)
	{
		if (m_pOrbit == NULL)
		{
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SLASHSWING);
			m_pOrbit = COrbit::Create(30, D3DXCOLOR(1.0f, 0.25f, 0.5f, 1.0f), D3DXVECTOR3(-1.5f, 0.0f, -54.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), m_apModel[15]->GetMatrixAddress(), "data\\TEXTURE\\OrbitBrade.png");
		}
		if (m_pAttackCollision == NULL)
		{
			m_pAttackCollision = CSphereCollision::Create(CSphereCollision::TYPE_ENEMYATTACK, 30.0f, 7, D3DXVECTOR3(-1.5f, 0.0f, -34.0f), VECTO3ZERO, m_apModel[15]->GetMatrixAddress(), this);
		}

	}
	if (m_pMotion->GetKey() >= 5)
	{
		if (m_pOrbit != NULL)
		{
			m_pOrbit->end();
			m_pOrbit = NULL;
		}
		if (m_pAttackCollision != NULL)
		{
			delete m_pAttackCollision;
			m_pAttackCollision = NULL;
		}
	}
	else
	{
		for (int i = 0; i < 14; i++)
		{
			if (m_apModel[i] != NULL)
			{
				CAfterImageObject::Create((char *)m_apModel[i]->GetName().c_str(), m_apModel[i]->GetMatrix(), D3DXCOLOR(1.0f, 0.25f, 0.5f, 0.25f), 30);
			}

		}
	}


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




CEnemy_Boss::CEnemy_Boss() :CEnemy()
{
	m_type = TYPE_BOSS;
}

CEnemy_Boss::~CEnemy_Boss()
{
}

//=============================================
//初期化関数
//=============================================
HRESULT CEnemy_Boss::Init()
{
	CEnemy::Init();
	m_pMotion->Load("data\\TEXT\\motion_boss.txt");
	m_pMotion->SetType(0);
	m_nCoolTime = 0;
		m_bMotionLock = false;
	m_pCollision = CSphereCollision::Create(CSphereCollision::TYPE_ENEMY, 200.0f, 0, D3DXVECTOR3(0.0f, 0.0f, 0.0f), VECTO3ZERO, m_apModel[0]->GetMatrixAddress(), this);
	m_pAttackCollision = NULL;
	m_posDest.x = GetPos().x;
	m_posDest.z = GetPos().z;
	m_posDest.y = GetPos().y;
	m_rotDest = (D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	m_pGage = CGage::Create(D3DXVECTOR3(14.0f,650.0f, 0.0f), 1000.0f, m_nLife);
	m_nRoutineCount = 0;
	m_Routine = ROUTINE_WAIT;
	return S_OK;
}
//=============================================
//終了関数
//=============================================
void CEnemy_Boss::Uninit()
{
	if (m_pAttackCollision != NULL)
	{
		delete m_pAttackCollision;
		m_pAttackCollision = NULL;
	}
	CEnemy::Uninit();

}
//=============================================
//更新関数
//=============================================
void CEnemy_Boss::Update()
{

	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	CMeshfield * pMesh = CManager::GetInstance()->GetScene()->GetMeshfield();

	
	m_pGage->SetData(m_nLife);

	m_nCoolTime--;
	D3DXVECTOR3 vec = pPlayer->GetPos() - GetPos();
	float fDis = GetDistance(vec);
	D3DXVec3Normalize(&vec, &vec);
	vec.y = 0.0f;
	if (fDis < 500.0f )
	{

		if (m_nCoolTime <= 0 && !m_bMotionLock && m_bLand)
		{
			m_nCoolTime = 120;
			if (rand() % 2 == 0)
			{
				m_pMotion->SetType(MOTION_ATTACK);
			}
			else
			{
				m_pMotion->SetType(MOTION_JUMP);
			}
		}
	}
	else if(fDis > 1500.0f)
	{
		if (m_nCoolTime <= 0 && m_bLand)
		{
			if (rand() % 2 == 0)
			{
				m_bTrigger = true;
				m_pMotion->SetType(MOTION_WALK);
				m_nCoolTime = 60;
			}
			else
			{
				m_pMotion->SetType(MOTION_NONE);
				CAnimBillboard::Create(800.0f, 800.0f, 34, 5, 170, 60, false, GetPos() + m_apModel[0]->GetPos(), "data\\TEXTURE\\beams_00000.jpg");
				CExplosion::Create(pPlayer->GetPos(), 120);
				CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_BEAM);
				m_nCoolTime = 300;


			}
		}
	}
	else
	{
		if (m_nCoolTime <=0)
		{
			CMissile::Create(GetPos() + m_apModel[0]->GetPos(), vec, 150, CBullet::TYPE_ENEMY, 7.0f, 0.05f, pPlayer);
			m_nCoolTime = 20;
		}
		
	}
	
	switch (m_pMotion->GetType())
	{
	case MOTION_ROTATION:
	{
		if (m_pAttackCollision != NULL)
		{
			delete m_pAttackCollision;
			m_pAttackCollision = NULL;
		}
		m_bMotionLock = false;
		SetMove(GetMove() + vec * 0.2f);
		m_rotDest.y = atan2f(-vec.x, -vec.z);
		D3DXVECTOR3 animRot = m_rotDest - GetRot();
		if (animRot.y < -D3DX_PI)
		{
			animRot.y += D3DX_PI * 2;
		}
		else if (animRot.y > D3DX_PI)
		{
			animRot.y += -D3DX_PI * 2;
		}
		SetRot(GetRot() + animRot * 0.05f);
	}
		break;
	case MOTION_ATTACK:
		Attack();
		break;
	case MOTION_JUMP:
		Jump();
		break;
	case MOTION_WALK:
		Charge();
		if (m_nCoolTime <= 0)
		{
			m_pMotion->SetType(MOTION_ROTATION);
		}
		break;
	default:
		m_pMotion->SetType(MOTION_ROTATION);
		m_bMotionLock = false;
		break;
	}
	
	m_pMotion->Update();
	m_nRoutineCount--;

	
	

	D3DXVECTOR3 move = GetMove();
		move.y -= GRAVITY ;
		SetMove(move);
		SetPos(GetPos() + GetMove());
		if (GetPos().y <= 0.0f)
		{
			D3DXVECTOR3 pos = GetPos();
			pos.y = 0.0f;
			move.y = 0.0f;
			SetPos(pos);
			m_bLand = true;


		

		}
	CEnemy::Update();
}

//=============================================
//描画関数
//=============================================
void CEnemy_Boss::Draw()
{
	CEnemy::Draw();
}
bool CEnemy_Boss::Damage(int nDamage, D3DXVECTOR3 knockback)
{

	if (CEnemy::Damage(nDamage, knockback))
	{

		return true;
	}
	return false;

}
void  CEnemy_Boss::Attack()
{
	m_bMotionLock = true;
	m_pMotion->SetType(MOTION_ATTACK);
	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	D3DXVECTOR3 vec = (pPlayer->GetPos() - GetPos());
	if (m_pAttackCollision == NULL)
	{
		D3DXVECTOR3 animRot = m_rotDest - GetRot();
		if (animRot.y < -D3DX_PI)
		{
			animRot.y += D3DX_PI * 2;
		}
		else if (animRot.y > D3DX_PI)
		{
			animRot.y += -D3DX_PI * 2;
		}
		SetRot(GetRot() + animRot * 0.05f);
	}

	if (m_pMotion->GetKey() == 3)
	{
	
		if (m_pAttackCollision == NULL)
		{
			m_pAttackCollision = CSphereCollision::Create(CSphereCollision::TYPE_ENEMYATTACK, 120.0f, 7, D3DXVECTOR3(0.0f, -50.0f, 0.0f), VECTO3ZERO, m_apModel[6]->GetMatrixAddress(), this);
		}

	}
	if (m_pMotion->GetKey() >= 8)
	{
		if (m_pAttackCollision != NULL)
		{
			delete m_pAttackCollision;
			m_pAttackCollision = NULL;
		}
	}
	else
	{
	
				CAfterImageObject::Create((char *)m_apModel[6]->GetName().c_str(), m_apModel[6]->GetMatrix(), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f), 30);
		
	}


}
void  CEnemy_Boss::Charge()
{
	m_pMotion->SetType(MOTION_WALK);
	if (m_bTrigger)
	{
		CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();

		D3DXVECTOR3 vec = (pPlayer->GetPos() - GetPos());
		vec.y = 0.0f;
		D3DXVec3Normalize(&vec, &vec);
		m_bTrigger = false;
		m_vec = vec;
		if (m_pAttackCollision == NULL)
		{
			m_pAttackCollision = CSphereCollision::Create(CSphereCollision::TYPE_ENEMYATTACK, 150.0f, 20, D3DXVECTOR3(0.0f, 0.0f, -50.0f), VECTO3ZERO, m_apModel[0]->GetMatrixAddress(), this);
		}
	}
	m_rotDest.y = atan2f(-m_vec.x, -m_vec.z);
	D3DXVECTOR3 animRot = m_rotDest - GetRot();
	if (animRot.y < -D3DX_PI)
	{
		animRot.y += D3DX_PI * 2;
	}
	else if (animRot.y > D3DX_PI)
	{
		animRot.y += -D3DX_PI * 2;
	}
	SetRot(GetRot() + animRot * 0.05f);
	D3DXVECTOR3 move = GetMove();


	
		move.x += m_vec.x * 1.5f;
		move.z += m_vec.z * 1.5f;
		for (int i = 0; i < 13; i++)
		{
			//CAfterImageObject::Create((char *)m_apModel[i]->GetName().c_str(), m_apModel[i]->GetMatrix(), D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.5f), 30);
		}

	SetMove(move);
}
void  CEnemy_Boss::Jump()
{
	m_bMotionLock = true;
	m_pMotion->SetType(MOTION_JUMP);
	CPlayer * pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	D3DXVECTOR3 vec = (pPlayer->GetPos() - GetPos());
	vec.y = 0.0f;
	D3DXVec3Normalize(&vec, &vec);
	m_rotDest.y = atan2f(-vec.x, -vec.z);
	D3DXVECTOR3 animRot = m_rotDest - GetRot();
	if (animRot.y < -D3DX_PI)
	{
		animRot.y += D3DX_PI * 2;
	}
	else if (animRot.y > D3DX_PI)
	{
		animRot.y += -D3DX_PI * 2;
	}
	SetRot(GetRot() + animRot * 0.05f);
	if (m_pMotion->GetKey() == 2 && m_bTrigger && m_bLand)
	{
		D3DXVECTOR3 move = GetMove();
	
		move.y += 20.0f;
		if (m_Routine == ROUTINE_FALLDOWN)
		{
			move.x += vec.x * 10.f;
			move.z += vec.z * 10.f;
		}
		else
		{
			move.x += vec.x * -50.f;
			move.z += vec.z * -50.f;
		}
		SetMove(move);
		m_bTrigger = false;
	}
	if (m_pMotion->GetKey() >= 2)
	{
	
		D3DXVECTOR3 move = GetMove();
		if (m_Routine == ROUTINE_FALLDOWN)
		{
		
			if (m_pMotion->GetKey() < 4)
			{
				move.y -= GRAVITY * 3;
				if (m_pAttackCollision == NULL)
				{
					m_pAttackCollision = CSphereCollision::Create(CSphereCollision::TYPE_ENEMYATTACK, 150.0f, 20, D3DXVECTOR3(0.0f, -50.0f, 0.0f), VECTO3ZERO, m_apModel[0]->GetMatrixAddress(), this);
				}
			}
			else
			{
				move.y += GRAVITY * 3;
			}
		
				move.x += vec.x * 6.f;
				move.z += vec.z * 6.f;
				
				for (int i = 0; i < 13; i++)
				{
					CAfterImageObject::Create((char *)m_apModel[i]->GetName().c_str(), m_apModel[i]->GetMatrix(), D3DXCOLOR(1.0f, 0.5f, 0.5f, 0.5f), 30);
				}
		}
		else
		{
			move.x += -vec.x * 1.f;
			move.z += -vec.z * 1.f;
			for (int i = 0; i < 13; i++)
			{
				CAfterImageObject::Create((char *)m_apModel[i]->GetName().c_str(), m_apModel[i]->GetMatrix(), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f), 30);
			}
		}
	
		SetMove(move);
	
	
	}
	
}
//=============================================
//生成関数
//=============================================
CEnemy_Boss * CEnemy_Boss::Create(D3DXVECTOR3 pos, int nLife)
{

	CEnemy_Boss * pEnemy = NULL;
	pEnemy = DBG_NEW  CEnemy_Boss;

	pEnemy->SetPos(pos);
	pEnemy->m_nLife = nLife;
	pEnemy->Init();

	return pEnemy;
}