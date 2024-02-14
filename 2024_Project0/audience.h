//==================
//プレイヤーヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _Audience_H_//二重インクルード防止のマクロ
#define _Audience_H_
//インクルード
#include "main.h"
#include "object.h"
#include "model.h"
#include "motion.h"
#include "orbit.h"
#include "collision.h"
#include "orbit.h"
#include "objectX.h"
#include <vector>
//マクロ定義

//クラス定義
class CAudience : public CObject //オブジェクトクラスの継承
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
	STATE GetState() { return m_state; }
	void MissileShower();
	void Concession();
	void Swarm();

	void SetActive(bool active) {m_bActive = active;}
private:
	typedef void(CAudience::*FunctionPointer)(void);
	FunctionPointer m_pAutoAttack;
	FunctionPointer m_pActiveAttack;
	int m_nBorder;
	int m_nStateCount;
	STATE m_state;
	CObjectX * m_pModel;
	D3DXVECTOR3 m_Offset;
	int m_nOffsetCount;
	D3DXVECTOR3 m_PosDest;
	D3DXVECTOR3 m_RotDest;
	COrbit * m_pOrbit;
	COrbit * m_pOrbit2;
	bool m_bActive;

};

#endif // ! _Audience_H_
