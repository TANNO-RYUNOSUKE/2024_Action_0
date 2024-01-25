//==================
//�r���{�[�h�w�b�_�[
//Author: �O�� ���V��
//==================
#ifndef  _AnimBillboard_H_//��d�C���N���[�h�h�~�̃}�N��
#define _AnimBillboard_H_
//�C���N���[�h
#include "billboard.h"

//�N���X��`
class CAnimBillboard : public CBillboard //�I�u�W�F�N�g�N���X�̌p��
{
public:

	CAnimBillboard(int nPriority = 1);

	~CAnimBillboard();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAnimBillboard * Create(float fHeight, float fWidth, int nH_Key, int nW_Key, int nNumKey, int nFlame, bool bLoop, D3DXVECTOR3 pos, char * pName = NULL);

private:
	int m_nH_Key;
	int m_nW_Key;
	int m_nFlame;
	int m_nNumKey;
	bool m_bLoop;
};

#endif // ! _AnimBillboard_H_
