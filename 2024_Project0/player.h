//==================
//�v���C���[�w�b�_�[
//Author: �O�� ���V��
//==================
#ifndef  _PLAYER_H_//��d�C���N���[�h�h�~�̃}�N��
#define _PLAYER_H_
//�C���N���[�h
#include "main.h"
#include "object.h"
#include "model.h"
#include "motion.h"
#include "orbit.h"
#include "collision.h"
#include "enemy.h"
#include "billboard.h"
#include <vector>
//�}�N����`
#define GRAVITY (0.8f)
#define WALK_SPEED (0.18f)
#define DASH_SPEED (1.0f)
#define ENERGY_MAX (3.0f)
//�N���X��`
class CPlayer : public CObject //�I�u�W�F�N�g�N���X�̌p��
{
public:
	CPlayer(int nPriority = 5);
	~CPlayer();

	typedef enum 
	{
		MOTION_NONE = -1,
		MOTION_NEUTRAL,
		MOTION_WALK,
		MOTION_DASH,
		
		MOTION_COMBINATION1,
		MOTION_COMBINATION2,
		MOTION_COMBINATION3,
		MOTION_COMBINATION4,
		
		MOTION_MAX
	}MOTION;



	typedef enum 
	{
		STATE_NONE = 0,
		STATE_NEUTRAL,
		STATE_DASH,
		STATE_HOVER,
		STATE_DAMAGE,
		STATE_DEATH,
		STATE_MAX
	}STATE;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer * Create(void);

	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	void SetLand(bool bLand) { m_bLand = bLand; }

	void AddLife(int nAdd) { m_nLife += nAdd; }
	int GetLife() { return m_nLife; }

	CModel * GetModel(int nData = 0) { return m_apModel[nData]; }

	//���[�V�����n
	void Action();

	void Walk();
	void Dash();
	void Attack1();
	void Attack2();
	void Attack3();
	void Attack4();
	void DeletCollision();
	void AutoCollisionCreate();
	void Direction();
	void Mirage();
	void Lockon();
private:
	
	int m_nLife;//�̗�

	CMotion * m_pMotion;//���[�V�����|�C���^
	COrbit *  m_pOrbit;
	D3DXVECTOR3 m_Force;//�O��
	D3DXVECTOR3 m_posOld;//1�t���[���O�̍��W
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_rotShot; //�ˌ�����
	bool m_bLand;//���n�������ǂ���
	bool m_bMotionLock;
	bool m_bKey;
	CModel * m_apModel[NUM_MODEL];//�g�p���郂�f���̃|�C���^
	int m_nNumModel;//�g�p���郂�f���̐�
	int   m_nDamage;
	float m_fPower;
	float m_Size;
	CEnemy ** m_pEnemy;
	CSphereCollision * m_pColl;
	CBillboard * m_pTarget;
};

#endif // ! _PLAYER_H_
