//==================
//レンダラーヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _RENDERER_H_//二重インクルード防止のマクロ
#define _RENDERER_H_
//インクルード
#include "main.h"
#include "object2D.h"
class CZTexture;	//Zテクスチャ
class CDepthShadow; //深度シャドウ
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
	CZTexture * GetZShader(void) { return m_pZTex; }
	CDepthShadow * GetDepthShader(void) { return m_pDepthShadow; }
private:
	ID3DXSprite * m_pSprite;	 //シャドウ用スプライト
	ID3DXSprite * m_pSpriteBloom;	 //シャドウ用スプライト
	CZTexture * m_pZTex;		//Zテクスチャ生成用
	CDepthShadow * m_pDepthShadow;					//深度影
	LPDIRECT3D9 m_pD3D;						//direct3Dへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//direct3Dデバイスへのポインタ
	IDirect3DDevice9 * m_pDev; //描画デバイス
	D3DXVECTOR3 Vdest, Rdest,V,R;
	D3DXMATRIX CameraView, CameraProj;	// カメラビュー変換・射影変換
	D3DXMATRIX LightView, LightProj;	// ライトビュー変換・射影変換
	LPDIRECT3DTEXTURE9 pSceneTexture;
	LPDIRECT3DTEXTURE9 pBloomTexture;
	LPD3DXEFFECT pEffectBrightCheck;//輝度確認
	LPD3DXEFFECT pGauss;//ぼかし
	IDirect3DTexture9 * m_pZTexture;				//Zテクスチャ本体
	CObject2D * m_pScreen;
	CObject2D * m_pMini;
	CObject2D * m_pBloom;
	CObject2D * m_pBlomMini;
};

#endif // ! _RENDERER_H_
