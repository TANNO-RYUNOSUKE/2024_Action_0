//==================
//テクスチャヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _XFILE_H_//二重インクルード防止のマクロ
#define _XFILE_H_
//インクルード
#include "main.h"
#include <string>

//マクロ定義
#define NUM_XFILE (25600) //テクスチャの数

//クラス定義
class CXFile
{
public:
	CXFile();
	~CXFile();

	HRESULT Load(void);
	void Unload(void);

	int Regist(std::string pFilename);
	LPD3DXMESH GetAddress(int nIdx);
	LPD3DXBUFFER  GetMaterial(int nIdx);
	DWORD GetNumMaterial(int nIdx) { return m_dwNumMat[nIdx]; }
private:
	LPD3DXMESH m_pMesh[NUM_XFILE];		//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER  m_pBuffMat[NUM_XFILE];	//マテリアルへのポインタ
	DWORD m_dwNumMat[NUM_XFILE];			//マテリアルの数
	std::string pName[NUM_XFILE];
	static int m_nNumAll; //テクスチャの総数
};


#endif // ! _XFILE_H_