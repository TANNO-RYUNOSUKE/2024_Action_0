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

	CAnimBillboard(int nPriority = 4);

	~CAnimBillboard();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetAnim(void);
	static CAnimBillboard * Create(float fHeight, float fWidth, int nH_Key, int nW_Key, int nNumKey, int nFlame, bool bLoop, D3DXVECTOR3 pos, char * pName = NULL);

private:
	int m_nH_Key;
	int m_nW_Key;
	
	int m_nNumKey;
	int m_nCurrentKey;
	float m_fFlameCount;
	int m_nFlameRate;
	bool m_bLoop;
};

#endif // ! _AnimBillboard_H_
