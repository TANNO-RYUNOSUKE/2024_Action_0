//==================
//ライフゲージヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _LIFEGAGE_H_//二重インクルード防止のマクロ
#define _LIFEGAGE_H_
//インクルード
#include "main.h"
#include "object2D.h"

//クラス定義
class CGage : public CObject //オブジェクトクラスの継承
{
public:
	CGage();
	~CGage();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGage * Create(D3DXVECTOR3 pos, float fWidth,int nMax);
	void SetData(int data) { m_ndata = data; }
private:

	CObject2D * m_pGage;
	CObject2D * m_pGageRed;
	float m_fWidth;
	int m_nMax;
	int m_ndata;
};

#endif // ! _LIFEGAGE_H_