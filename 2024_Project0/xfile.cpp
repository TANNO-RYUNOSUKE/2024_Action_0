//========================================================================================
//
// テクスチャ
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "XFile.h"
#include "manager.h"
#include <string.h>
//静的メンバの初期化
int CXFile::m_nNumAll = 0;

CXFile::CXFile()
{
	for (int nCnt = 0; nCnt < NUM_XFILE; nCnt++)
	{
		m_pMesh[nCnt] = NULL;
		m_pBuffMat[nCnt] = NULL;
		m_dwNumMat[nCnt] = 0;
		pName[nCnt] = {};
	}
}

CXFile::~CXFile()
{
}
//=============================================
//ロード関数
//=============================================
HRESULT CXFile::Load(void)
{
	return S_OK;
}
//=============================================
//アンロード関数
//=============================================
void CXFile::Unload(void)
{
	for (int nCnt = 0; nCnt < NUM_XFILE; nCnt++)
	{
		m_pBuffMat[nCnt] = NULL;
		m_pMesh[nCnt] = NULL;
		m_nNumAll = 0;
	}
}
//=============================================
//登録関数
//=============================================
int CXFile::Regist(std::string pFilename)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	int nIdx = m_nNumAll;
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{

		if (strcmp(pName[nCnt].c_str(), pFilename.c_str()) == 0)
		{
			return nCnt;
		}
	}
	if (m_pMesh[nIdx] == NULL)
	{
		//テクスチャ読み込み
		if (D3DXLoadMeshFromX(pFilename.c_str(),
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMat[nIdx],
			NULL,
			&m_dwNumMat[nIdx],
			&m_pMesh[nIdx]) != D3D_OK)
		{
			return E_FAIL;
		}
		pName[nIdx] = std::string(pFilename);
		m_nNumAll++;
		return nIdx;
	}
	else
	{
		return -1;
	}

}
//=============================================
//アドレス取得関数
//=============================================
LPD3DXMESH CXFile::GetAddress(int nIdx)
{
	if (nIdx < 0)
	{
		return NULL;
	}
	if (m_pMesh[nIdx] != NULL)
	{
		return m_pMesh[nIdx];
	}
	return NULL;
}
LPD3DXBUFFER CXFile::GetMaterial(int nIdx)
{
	if (nIdx < 0)
	{
		return NULL;
	}
	if (m_pMesh[nIdx] != NULL)
	{
		return m_pBuffMat[nIdx];
	}
	return NULL;
}