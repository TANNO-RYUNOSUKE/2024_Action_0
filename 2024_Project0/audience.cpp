//============================================================
//
//	�U���r�b�g�Ǘ� [audience.cpp]
//	Author�F�O�엳�V��
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
//====================================
//�R���X�g���N�^�A�f�X�g���N�^
//====================================
CAudience::CAudience(int nPriority) : CObject(nPriority)
{
	m_nStateCount = 0;
	m_RotDest = VECTO3ZERO;
	m_PosDest = VECTO3ZERO;
	m_state = STATE_NONE;
	m_pModel = NULL;
}

CAudience::~CAudience()
{
}

//====================================
//������
//====================================
HRESULT CAudience::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	m_pModel = CObjectX::Create("data\\MODEL\\Audience.x",VECTO3ZERO);
	m_state = STATE_NEUTRAL;
	m_pOrbit = COrbit::Create(60, D3DXCOLOR(0.7f, 1.0f, 0.3f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), GetMatrixAddress());
	return S_OK;
}
//====================================
//�I��
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
}
//====================================
//�X�V
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
//�`��
//====================================
void CAudience::Draw(void)
{	
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���N�X

	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 pos = GetPos();
	//���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtx);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);

	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);

	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxTrans);

	m_pModel->Draw();
}
//====================================
//�U��
//====================================
void CAudience::Attack()
{
	m_nStateCount++;
	if (m_nStateCount % 120 == 0)
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
}
//====================================
//����
//====================================
CAudience * CAudience::Create(void)
{
	CAudience * pAudience = DBG_NEW CAudience;
	pAudience->Init();
	return pAudience;
}