#include "XManager.h"
CObjectX *CXManager::m_apObjectX[NUM_OBJECTX] = {};

//==============================
//コンストラクタ,デストラクタ
//==============================
CXManager::CXManager()
{

}
CXManager::~CXManager()
{

}
void CXManager::Init(void)
{
	m_nNumALL = 0;
	for (int nCnt = 0; nCnt < NUM_OBJECTX; nCnt++)
	{
		m_apObjectX[nCnt] = NULL;
	}

}
//==============================
//登録処理
//==============================
void CXManager::Regist(CObjectX * pObj)
{
	for (int nCnt = 0; nCnt < NUM_OBJECTX; nCnt++)
	{
		if (m_apObjectX[nCnt] == NULL)
		{
			m_apObjectX[nCnt] = pObj;
			m_apObjectX[nCnt]->SetID(nCnt);
			m_nNumALL++;
			break;
		}
	}
}
//==============================
//解放処理
//==============================
void CXManager::Release(int nID)
{
	if (m_apObjectX[nID] != NULL)
	{
		m_apObjectX[nID] = NULL;
		m_nNumALL--;
	}
}
