//========================================================================================
//
// �A�j���[�V�����r���{�[�h
// Author: �O�� ���V��
//
//========================================================================================
//�C���N���[�h
#include "renderer.h"
#include "object3D.h"
#include "manager.h"
#include "AnimBillboard.h"
#include "texture.h"

//=============================================
//�R���X�g���N�^
//=============================================

CAnimBillboard::CAnimBillboard(int nPriority) : CBillboard(nPriority)
{

}



//=============================================
//�f�X�g���N�^
//=============================================
CAnimBillboard::~CAnimBillboard()
{
}


//=============================================
//�|���S���������֐�
//=============================================
HRESULT CAnimBillboard::Init(void)
{

	
	return S_OK;;
}
//=============================================
//�����֐�
//=============================================
CAnimBillboard * CAnimBillboard::Create(float fHeight, float fWidth, int nH_Key, int nW_Key, int nNumKey, int nFlame, bool bLoop, D3DXVECTOR3 pos, char * pName )
{
	CAnimBillboard * pObject3D = NULL;

	pObject3D = DBG_NEW CAnimBillboard;
	if (pName != NULL)
	{
		CTexture * pTex = CManager::GetInstance()->GetTexture();

		pObject3D->m_nIdxTex[0] = pTex->Regist(pName);
	}

	pObject3D->SetHeight(fHeight);
	pObject3D->SetWidth(fWidth);
	pObject3D->SetPos(pos);
	pObject3D->Init();
	return pObject3D;
}



//=============================================
//�|���S���̏I������
//=============================================
void CAnimBillboard::Uninit(void)
{

	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================
//�|���S���̍X�V����
//=============================================
void CAnimBillboard::Update(void)
{
	SetVtx();
}

//=============================================
//�|���S���̕`�揈��
//=============================================
void CAnimBillboard::Draw(void)
{


}
