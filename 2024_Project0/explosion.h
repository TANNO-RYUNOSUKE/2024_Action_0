//==================
//ライフゲージヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _explosion_H_//二重インクルード防止のマクロ
#define _explosion_H_
//インクルード
#include "main.h"
#include "object2D.h"
#include "animbillboard.h"
#include "collision.h"
//クラス定義
class CExplosion : public CObject //オブジェクトクラスの継承
{
public:
	CExplosion();
	~CExplosion();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CExplosion * Create(D3DXVECTOR3 pos,int nLife);
	void SetLife(int n) { m_nLife = n; }
private:
	CSphereCollision * pCol;
	int m_nLife;
};

#endif // ! _explosion_H_