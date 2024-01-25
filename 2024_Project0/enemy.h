//==================
//�G�l�~�[�w�b�_�[
//Author: �O�� ���V��
//==================
#ifndef  _ENEMY_H_//��d�C���N���[�h�h�~�̃}�N��
#define _ENEMY_H_
//�C���N���[�h
#include "main.h"
#include "object.h"
#include "model.h"

#include "motion.h"
#include "orbit.h"
#include "collision.h"


//�N���X��`
class CEnemy : public CObject //�I�u�W�F�N�g�N���X�̌p��
{
public:
	CEnemy();
	~CEnemy();

	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_TEST,
		TYPE_MAX
	};
	enum STATE
	{
		STATE_NONE = 0,
		STATE_DAMAGE,
		STATE_DEAD,
		STATE_MAX
	};

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CEnemy * Create(D3DXVECTOR3 pos,  int nLife);

	CModel *GetModel(void) { return m_apModel[0]; }

	
	void AddLife(int nAdd) { m_nLife += nAdd; }//���C�t�ɉ��Z
	bool Damage(int nDamage, D3DXVECTOR3 knockback);
	void SetState(STATE state, int nCnt) { m_state = state,m_nStateCount = nCnt; }
protected:
	D3DXVECTOR3 posOld;
	STATE m_state;
	int m_nStateCount;
	float m_fdIstance;//�v���C���[�Ƃ̋���
	int m_nLife; //�̗�
	D3DXVECTOR3 m_posDest;
	D3DXVECTOR3 m_Force;
	D3DXVECTOR3 m_rotDest;
	CMotion * m_pMotion;//���[�V�����|�C���^
	CModel * m_apModel[NUM_MODEL];//�g�p���郂�f���̃|�C���^
	
private:
	
};
//�G�l�~�[(�e�X�g)
class CEnemy_TEST : public CEnemy
{
public:
	CEnemy_TEST();
	~CEnemy_TEST();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemy_TEST * Create(D3DXVECTOR3 pos, int nLife);
private:
	CSphereCollision * m_pCollision;
};


#endif // ! _ENEMY_H_
