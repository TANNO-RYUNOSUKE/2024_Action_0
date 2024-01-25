//==================
//コリジョンヘッダー
//Author: 丹野 竜之介
//==================
#include "main.h"
#include "list.h"
#include "object.h"
#include "enemy.h"
#ifndef _Collision_H_
#define _Collision_H_

class CSphereCollision
{
public:
	CSphereCollision();
	~CSphereCollision();

	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_PLAYERATTACK,
		TYPE_ENEMYATTACK,
		TYPE_INDISCRIMINATEATTACK,
		TYPE_MAX
	};

	void Collision();
	static CSphereCollision * Create(TYPE type,float fRadius,int nPower, D3DXVECTOR3 Offset, D3DXVECTOR3 knockback,D3DXMATRIX * pMtx = NULL, CObject * Parent = NULL);
	void SetRadius(float Radius) { m_fRadius = Radius; }
	float GetRadius() { return m_fRadius; }
	void SetKnockback(D3DXVECTOR3 vec) { m_knockback = vec; }
	static Clist<CSphereCollision *> List;
private:
	
	int m_nPower;
	float m_fRadius;
	D3DXMATRIX * m_pMtx;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Offset;
	D3DXVECTOR3 m_knockback;
	CObject * m_pParent;
	TYPE m_Type;
};



#endif // !_Collision_H_
