#include "collision.h"
#include "manager.h"
Clist<class CSphereCollision *> CSphereCollision::List = {};
CSphereCollision::CSphereCollision()
{
	
	List.Regist(this);
}

CSphereCollision::~CSphereCollision()
{
	
}

void CSphereCollision::Collision()
{

	D3DXMATRIX MtxOffset;
	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&MtxOffset);
	D3DXMatrixTranslation(&MtxOffset, m_Offset.x, m_Offset.y, m_Offset.z);
	D3DXMatrixMultiply(&MtxOffset, &MtxOffset, m_pMtx);
	m_Pos = { MtxOffset._41,MtxOffset._42, MtxOffset._43 };
	switch (m_Type)
	{
	case CSphereCollision::TYPE_NONE:
		break;
	case CSphereCollision::TYPE_PLAYER:
		break;
	case CSphereCollision::TYPE_ENEMY:
		break;
	case CSphereCollision::TYPE_PLAYERATTACK:
		for (int i = 0; i < (int)List.GetList().size(); i++)
		{
			CSphereCollision * pCollision = *std::next(List.GetList().begin(), i);
			if (pCollision->m_Type == TYPE_ENEMY)
			{
				
				if (CManager::GetInstance()->GetDistance(pCollision->m_Pos - m_Pos) <= (m_fRadius + pCollision->m_fRadius))
				{
					pCollision->m_pParent->SetLife(pCollision->m_pParent->GetLife() - m_nPower);
					pCollision->m_pParent->SetMove(pCollision->m_pParent->GetMove() + m_knockback);
				}
			}
		}
		break;
	case CSphereCollision::TYPE_ENEMYATTACK:
		break;
	case CSphereCollision::TYPE_INDISCRIMINATEATTACK:
		break;
	case CSphereCollision::TYPE_MAX:
		break;
	default:
		break;
	}
}

CSphereCollision * CSphereCollision::Create(TYPE type, float fRadius, int nPower, D3DXVECTOR3 Offset, D3DXVECTOR3 knockback, D3DXMATRIX * pMtx, CObject * Parent)
{
	CSphereCollision * pOrbit = NULL;

	pOrbit = DBG_NEW CSphereCollision();

	pOrbit->m_Type = type;
	pOrbit->m_fRadius = fRadius;
	pOrbit->m_nPower = nPower;
	pOrbit->m_Offset = Offset;
	pOrbit->m_knockback = knockback;
	pOrbit->m_pMtx = pMtx;
	return pOrbit;
}