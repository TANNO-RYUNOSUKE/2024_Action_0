//==================
//レンダラーヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _RENDERER_H_//二重インクルード防止のマクロ
#define _RENDERER_H_
//インクルード
#include "main.h"

//クラス定義
class CRenderer //レンダラークラス
{
public:
	//コンストラクタとデストラクタ
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void){return m_pD3DDevice;}

private:						
	LPDIRECT3D9 m_pD3D;						//direct3Dへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//direct3Dデバイスへのポインタ
};

#endif // ! _RENDERER_H_
