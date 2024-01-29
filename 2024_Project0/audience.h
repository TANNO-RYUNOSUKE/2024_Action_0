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

	typedef enum
	{
		STATE_NONE = 0,
		STATE_NEUTRAL,
		STATE_AUTOATTACK,
		STATE_MANUALATTACK,
		STATE_DEATH,
		STATE_MAX
	}STATE;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Attack();
	static CAudience * Create(void);
	void SetState(STATE state, int nCnt) { m_state = state, m_nStateCount = nCnt; }
private:
	int m_nStateCount;
	STATE m_state;
	CObjectX * m_pModel;
	D3DXVECTOR3 m_PosDest;
	D3DXVECTOR3 m_RotDest;
	COrbit * m_pOrbit;
};

#endif // ! _Audience_H_
