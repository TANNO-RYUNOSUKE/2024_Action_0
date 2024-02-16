//==================
//エネミーヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _ENEMY_H_//二重インクルード防止のマクロ
#define _ENEMY_H_
//インクルード
#include "main.h"
#include "object.h"
#include "model.h"

#include "motion.h"
#include "orbit.h"
#include "lifegage.h"
#include "collision.h"
#include "light.h"

//クラス定義
class CEnemy : public CObject //オブジェクトクラスの継承
{
public:
	CEnemy();
	~CEnemy();

	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_TEST,
		TYPE_ARMY,
		TYPE_BOSS,
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

	
	void AddLife(int nAdd) { m_nLife += nAdd; }//ライフに加算
	virtual bool Damage(int nDamage, D3DXVECTOR3 knockback);
	void SetState(STATE state, int nCnt) { m_state = state,m_nStateCount = nCnt; }

	static Clist<CEnemy *> EnemyList;
protected:
	D3DXVECTOR3 posOld;
	STATE m_state;
	int m_nStateCount;
	float m_fdIstance;//プレイヤーとの距離
	int m_nLife; //体力
	int m_nArmor;
	int m_nArmorMax;
	D3DXVECTOR3 m_posDest;
	D3DXVECTOR3 m_Force;
	D3DXVECTOR3 m_rotDest;
	CMotion * m_pMotion;//モーションポインタ
	CModel * m_apModel[NUM_MODEL];//使用するモデルのポインタ
	CSphereCollision * m_pCollision;
	TYPE m_type;
private:
	
};
//エネミー(テスト)
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
	
};


//エネミー(歩行戦車)
class CEnemy_Walker : public CEnemy
{
public:
	CEnemy_Walker();
	~CEnemy_Walker();
	enum MOVE
	{
		MOVE_NONE = 0,
		MOVE_WALK,
		MOVE_DAMAGE,
		MOVE_MAX
	};
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemy_Walker * Create(D3DXVECTOR3 pos, int nLife);
private:
	MOVE m_Move;

};

//エネミー(一般)
class CEnemy_army : public CEnemy
{
public:
	CEnemy_army();
	~CEnemy_army();
	enum MOTION
	{
		MOTION_NONE = 0,
		MOTION_WALK,
		MOTION_ATTACK,
		MOTION_DAMAGE,
		MOTION_DOWN,
		MOTION_MAX
	};
	enum ROUTINE
	{
		ROUTINE_WAIT = 0,
		ROUTINE_FORWARD,
		ROUTINE_BACK,
		ROUTINE_ATTACK,
		ROUTINE_MAX
	};
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Walk();
	void Attack();
	bool Damage(int nDamage, D3DXVECTOR3 knockback);
	static CEnemy_army * Create(D3DXVECTOR3 pos, int nLife);
	void SetRoutine(ROUTINE Routine, int nCnt) { m_Routine = Routine; m_nRoutineCount = nCnt; }
private:
	ROUTINE m_Routine;
	COrbit * m_pOrbit;
	int m_nRoutineCount;
	CSphereCollision * m_pAttackCollision;

};
//エネミー(ボス)
class CEnemy_Boss : public CEnemy
{
public:
	CEnemy_Boss();
	~CEnemy_Boss();
	enum MOTION
	{
		MOTION_NONE = 0,
		MOTION_ROTATION,
		MOTION_WALK,
		MOTION_ATTACK,
		MOTION_JUMP,
		
		MOTION_MAX
	};
	enum ROUTINE
	{
		ROUTINE_WAIT = 0,
		ROUTINE_FORWARD,
		ROUTINE_ROTATION,
		ROUTINE_ATTACK,
		ROUTINE_JUMP,
		ROUTINE_CHARGE,
		ROUTINE_FALLDOWN,
		ROUTINE_BEAM,
		ROUTINE_MAX
	};
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Walk();
	void Jump();
	void Fall();
	void Charge();
	void Beam();
	void Attack();
	bool Damage(int nDamage, D3DXVECTOR3 knockback);
	static CEnemy_Boss * Create(D3DXVECTOR3 pos, int nLife);
	void SetRoutine(ROUTINE Routine, int nCnt) { m_Routine = Routine; m_nRoutineCount = nCnt; }
private:
	ROUTINE m_Routine;
	int m_nRoutineCount;
	int m_nCoolTime;
	bool m_bMotionLock;
	bool m_bLand;
	bool m_bTrigger;
	CGage * m_pGage;
	D3DXVECTOR3 m_vec;
	CSphereCollision * m_pAttackCollision;

};
#endif // ! _ENEMY_H_
