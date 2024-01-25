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
#include "collision.h"


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
	bool Damage(int nDamage, D3DXVECTOR3 knockback);
	void SetState(STATE state, int nCnt) { m_state = state,m_nStateCount = nCnt; }
protected:
	D3DXVECTOR3 posOld;
	STATE m_state;
	int m_nStateCount;
	float m_fdIstance;//プレイヤーとの距離
	int m_nLife; //体力
	D3DXVECTOR3 m_posDest;
	D3DXVECTOR3 m_Force;
	D3DXVECTOR3 m_rotDest;
	CMotion * m_pMotion;//モーションポインタ
	CModel * m_apModel[NUM_MODEL];//使用するモデルのポインタ
	
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
	CSphereCollision * m_pCollision;
};


#endif // ! _ENEMY_H_
