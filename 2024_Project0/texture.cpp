//========================================================================================
//
// テクスチャ
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include <string.h>
//静的メンバの初期化
int CTexture::m_nNumAll = 0;

CTexture::CTexture()
{
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		m_apTexture[nCnt] = NULL;
		m_pNameArray[nCnt] = NULL;
	}

}

CTexture::~CTexture()
{
	for (int i = 0; i < NUM_TEXTURE; i++)
	{
		if (m_pNameArray[i] !=NULL)
		{
			delete[]  m_pNameArray[i];
			m_pNameArray[i] = NULL;
		}
	
	}
}
//=============================================
//ロード関数
//=============================================
HRESULT CTexture::Load(void)
{
	return S_OK;
}
//=============================================
//アンロード関数
//=============================================
void CTexture::Unload(void)
{
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		m_apTexture[nCnt] = NULL;
		m_nNumAll = 0;
	}
}
//=============================================
//登録関数
//=============================================
int CTexture::Regist(char * pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	int nIdx = m_nNumAll;

	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		if (strcmp(m_pNameArray[nCnt], pFilename) == 0)
		{
			return nCnt;
		}
	}

	if (m_apTexture[nIdx] == NULL)
	{
		// テクスチャ読み込み
		D3DXCreateTextureFromFile(pDevice, pFilename, &m_apTexture[nIdx]);

		// ファイル名をコピーして保存
		m_pNameArray[nIdx] = new char[strlen(pFilename) + 1];
		strcpy(m_pNameArray[nIdx], pFilename);

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
LPDIRECT3DTEXTURE9 CTexture::Getaddress(int nIdx)
{
	if (nIdx < 0 || nIdx >= NUM_TEXTURE)
	{
		return NULL;
	}

	if (m_apTexture[nIdx] != NULL)
	{
		return m_apTexture[nIdx];
	}
	return NULL;
}
