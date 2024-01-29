//==================
//プレイヤーヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _PLAYER_H_//二重インクルード防止のマクロ
#define _PLAYER_H_
//インクルード
#include "main.h"
#include "object.h"
#include "model.h"
#include "motion.h"
#include "orbit.h"
#include "collision.h"
#include "enemy.h"
#include "billboard.h"
#include <vector>
//マクロ定義
#define GRAVITY (0.8f)
#define WALK_SPEED (0.18f)
#define DASH_SPEED (1.0f)
#define ENERGY_MAX (3.0f)
//クラス定義
class CPlayer : public CObject //オブジェクトクラスの継承
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

	//モーション系
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
	
	int m_nLife;//体力

	CMotion * m_pMotion;//モーションポインタ
	COrbit *  m_pOrbit;
	D3DXVECTOR3 m_Force;//外力
	D3DXVECTOR3 m_posOld;//1フレーム前の座標
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_rotShot; //射撃方向
	bool m_bLand;//着地したかどうか
	bool m_bMotionLock;
	bool m_bKey;
	CModel * m_apModel[NUM_MODEL];//使用するモデルのポインタ
	int m_nNumModel;//使用するモデルの数
	int   m_nDamage;
	float m_fPower;
	float m_Size;
	CEnemy ** m_pEnemy;
	CSphereCollision * m_pColl;
	CBillboard * m_pTarget;
};

#endif // ! _PLAYER_H_
