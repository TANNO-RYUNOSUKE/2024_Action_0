//========================================================================================
//
// �o���b�g
// Author: �O�� ���V��
//
//========================================================================================
//�C���N���[�h
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
//�R���X�g���N�^
//=============================================
CBullet::CBullet(int nPriority):CObject(nPriority)
{
	m_nLife = 0;

	m_Type = TYPE_NONE;

}
//=============================================
//�f�X�g���N�^
//=============================================
CBullet::~CBullet()
{
}

//=============================================
//�������֐�
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
//�I���֐�
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
//�X�V�֐�
//=============================================
void CBullet::Update()
{

	m_nLife--;
	SetPos(GetPos() + GetMove());
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
	if (m_nLife <= 0)
	{
		CObject::Release();
	}
	
}
//=============================================
//�`��֐�
//=============================================
void CBullet::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();


}
//=============================================
//�����֐�
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
