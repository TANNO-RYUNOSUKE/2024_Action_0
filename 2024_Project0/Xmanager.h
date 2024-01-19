//==================
//マネージャヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _XManager_H_//二重インクルード防止のマクロ
#define _XManager_H_
//インクルード
#include "main.h"
#include "objectX.h"

#define NUM_OBJECTX (128)//エネミーの最大数
//クラス定義
class CXManager //マネージャクラス
{
public:
	CXManager();
	~CXManager();

	void Init(void);
	void Regist(CObjectX * pObj);
	void Release(int nID);
	CObjectX ** GetX(void) { return &m_apObjectX[0]; }
	int GetNum(void) { return m_nNumALL; }

private:
	static CObjectX * m_apObjectX[NUM_OBJECTX];//エネミーの配列
	int m_nNumALL;//合計数
};

#endif // ! _XManager_H_
