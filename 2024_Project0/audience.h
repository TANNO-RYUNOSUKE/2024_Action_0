//==================
//�v���C���[�w�b�_�[
//Author: �O�� ���V��
//==================
#ifndef  _Audience_H_//��d�C���N���[�h�h�~�̃}�N��
#define _Audience_H_
//�C���N���[�h
#include "main.h"
#include "object.h"
#include "model.h"
#include "motion.h"
#include "orbit.h"
#include "collision.h"
#include "orbit.h"
#include "objectX.h"
#include <vector>
//�}�N����`

//�N���X��`
class CAudience : public CObject //�I�u�W�F�N�g�N���X�̌p��
{
public:
	CAudience(int nPriority = 3);
	~CAudience();

	enum STATE
	{
		STATE_NONE = 0,
		STATE_NEUTRAL,
		STATE_AUTOATTACK,
		STATE_MANUALATTACK,
		STATE_DEATH,
		STATE_MAX
	};

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Attack();
	static CAudience * Create(void);
	static Clist<CAudience *> List;
	static void SetStateAll(STATE state, int nCnt);
	void SetState(STATE state, int nCnt) { m_state = state, m_nStateCount = nCnt; }
private:
	int m_nBorder;
	int m_nStateCount;
	STATE m_state;
	CObjectX * m_pModel;
	D3DXVECTOR3 m_PosDest;
	D3DXVECTOR3 m_RotDest;
	COrbit * m_pOrbit;
	COrbit * m_pOrbit2;
};

#endif // ! _Audience_H_
