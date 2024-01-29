//==================
//�o���b�g�w�b�_�[
//Author: �O�� ���V��
//==================
#ifndef  _BULLET_H_//��d�C���N���[�h�h�~�̃}�N��
#define _BULLET_H_
//�C���N���[�h
#include "main.h"
#include "billboard.h"
#include "orbit.h"
#include "collision.h"
//�N���X��`
class CBullet : public CObject //�I�u�W�F�N�g�N���X�̌p��
{
public:
	CBullet(int nPriority = 2);
	CBullet(D3DXVECTOR3 pos,D3DXVECTOR3 move,int nLife);
	~CBullet();
	enum TYPE
	{
		TYPE_NONE =0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_MAX
	};
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBullet * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife,TYPE type);


private:
	TYPE m_Type;
	CSphereCollision * m_pColl;
	COrbit * m_pOrbit;
	int m_nLife;//����

};

#endif // ! _BULLET_H_