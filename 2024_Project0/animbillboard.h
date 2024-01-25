//==================
//ビルボードヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _AnimBillboard_H_//二重インクルード防止のマクロ
#define _AnimBillboard_H_
//インクルード
#include "billboard.h"

//クラス定義
class CAnimBillboard : public CBillboard //オブジェクトクラスの継承
{
public:

	CAnimBillboard(int nPriority = 1);

	~CAnimBillboard();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAnimBillboard * Create(float fHeight, float fWidth, int nH_Key, int nW_Key, int nNumKey, int nFlame, bool bLoop, D3DXVECTOR3 pos, char * pName = NULL);

private:
	int m_nH_Key;
	int m_nW_Key;
	int m_nFlame;
	int m_nNumKey;
	bool m_bLoop;
};

#endif // ! _AnimBillboard_H_
