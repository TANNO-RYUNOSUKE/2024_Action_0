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
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	static CBullet * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, TYPE type);
	void SetLife(int nLife) { m_nLife = nLife; }
protected:
	TYPE m_Type;
private:

	CSphereCollision * m_pColl;
	COrbit * m_pOrbit;
	COrbit * m_pOrbit2;
	CObject * m_pTarget;
	int m_nLife;//寿命

};

class CMissile : public CBullet
{
public:
	CMissile();
	~CMissile();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMissile * Create(D3DXVECTOR3 pos, D3DXVECTOR3 vec, int nLife, TYPE type,float fSpeed,float fPower,CObject * Target);
	void Homing();
private:
	float m_fSpeed; 
	float m_fPower;
	CObject ** m_pTarget;
};


#endif // ! _BULLET_H_