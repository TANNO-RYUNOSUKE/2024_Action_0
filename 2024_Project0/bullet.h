//==================
//バレットヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _BULLET_H_//二重インクルード防止のマクロ
#define _BULLET_H_
//インクルード
#include "main.h"
#include "billboard.h"
#include "orbit.h"
#include "collision.h"
//クラス定義
class CBullet : public CObject //オブジェクトクラスの継承
{
public:
	CBullet(int nPriority = 2);
	CBullet(D3DXVECTOR3 pos,D3DXVECTOR3 move,int nLife);
	~CBullet();
	enum TYPE
	{
		TYPE_NONE =0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_MAX
	};
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBullet * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife,TYPE type);


private:
	TYPE m_Type;
	CSphereCollision * m_pColl;
	COrbit * m_pOrbit;
	int m_nLife;//寿命

};

#endif // ! _BULLET_H_