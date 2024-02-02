#include "collision.h"
#include "manager.h"
#include "animbillboard.h"
#include "sound.h"
#include "camera.h"
#include "player.h"
Clist<CSphereCollision *> CSphereCollision::List = {};
CSphereCollision::CSphereCollision()
{
	
}

CSphereCollision::~CSphereCollision()
{
	List.Delete(this);
}

void CSphereCollision::Collision()
{
	CCamera * pCamera = CManager::GetInstance()->GetScene()->GetCamera();
	D3DXMATRIX MtxOffset;
	//ワールドマトリクスの初期化
	D3DXMatrixIdentity(&MtxOffset);
	D3DXMatrixTranslation(&MtxOffset, m_Offset.x, m_Offset.y, m_Offset.z);
	if (m_pMtx != NULL)
	{
		D3DXMatrixMultiply(&MtxOffset, &MtxOffset, m_pMtx);
	}

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
		for (int i = 0; i < (int)List.GetList()->size(); i++)
		{
			CSphereCollision * pCollision = *std::next(List.GetList()->begin(), i);
			if (pCollision->m_Type == TYPE_ENEMY)
			{	
				if (CManager::GetInstance()->GetDistance(pCollision->m_Pos - m_Pos) <= (m_fRadius + pCollision->m_fRadius))
				{
					if (pCollision->m_pParent->Damage(m_nPower, m_knockback))
					{
						pCamera->SetShake(3);
						CManager::GetInstance()->SetHitStop(3);
						CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SLASHHIT);
						CAnimBillboard::Create(m_nPower * 5.0f, m_nPower * 5.0f, 3, 6, 18, 60, false, (pCollision->m_Pos + m_Pos)*0.5f + m_knockback, "data\\TEXTURE\\HitEffect.png");
					}
				}
			}
		}
		break;
	case CSphereCollision::TYPE_AUDIENCEATTACK:
		for (int i = 0; i < (int)List.GetList()->size(); i++)
		{
			CSphereCollision * pCollision = *std::next(List.GetList()->begin(), i);
			if (pCollision->m_Type == TYPE_ENEMY)
			{
				if (CManager::GetInstance()->GetDistance(pCollision->m_Pos - m_Pos) <= (m_fRadius + pCollision->m_fRadius))
				{
					pCollision->m_pParent->SetLife(pCollision->m_pParent->GetLife() - m_nPower);

					CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SHOT);
					CAnimBillboard::Create(m_nPower * 5.0f, m_nPower * 5.0f, 3, 6, 18, 60, false, m_Pos + m_knockback, "data\\TEXTURE\\HitEffect2.png");
					m_pParent->SetLife(0);
					return;

				}
			}
		}
		break;
	case CSphereCollision::TYPE_ENEMYATTACK:
	{
		CSphereCollision * pCollision = CManager::GetInstance()->GetScene()->GetPlayer()->GetHitCol();

		if (CManager::GetInstance()->GetDistance(pCollision->m_Pos - m_Pos) <= (m_fRadius + pCollision->m_fRadius))
		{
			D3DXVECTOR3 vec = pCollision->m_Pos - m_Pos;
			vec.y = 0.0f;
			D3DXVec3Normalize(&vec, &vec);
			if (pCollision->m_pParent->Damage(m_nPower, -vec * 25.0f))
			{
				pCamera->SetShake(6);

				CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_SLASHHIT);
				CAnimBillboard::Create(m_nPower * 5.0f, m_nPower * 5.0f, 3, 6, 18, 60, false, (pCollision->m_Pos + m_Pos)*0.5f + m_knockback, "data\\TEXTURE\\HitEffect.png")->SetColor(D3DXCOLOR(1.0f, 0.25f, 0.5f, 1.0f));
			}
		}
	}
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
	pOrbit->m_pParent = Parent;
	List.Regist(pOrbit);
	return pOrbit;
}