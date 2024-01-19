//==================
//テクスチャヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _TEXTURE_H_//二重インクルード防止のマクロ
#define _TEXTURE_H_
//インクルード
#include "main.h"


//マクロ定義
#define NUM_TEXTURE (256) //テクスチャの数

//クラス定義
class CTexture
{
public:
	CTexture();
	~CTexture();

	HRESULT Load(void);
	void Unload(void);

	int Regist(char * pFilename);
	LPDIRECT3DTEXTURE9 Getaddress(int nIdx);

private:
	LPDIRECT3DTEXTURE9 m_apTexture[NUM_TEXTURE];//テクスチャの情報
	char * m_pNameArray[NUM_TEXTURE];
	static int m_nNumAll; //テクスチャの総数
};


#endif // ! _TEXTURE_H_
