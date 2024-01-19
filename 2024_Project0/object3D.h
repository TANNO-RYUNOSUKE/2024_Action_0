//==================
//3Dオブジェクトヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _OBJECT3D_H_//二重インクルード防止のマクロ
#define _OBJECT3D_H_
//インクルード
#include "main.h"
#include "object.h"

//クラス定義
class CObject3D : public CObject //オブジェクトクラスの継承
{
public:

	CObject3D(int nPriority = 0);

	~CObject3D();

	virtual	HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	float GetHeight(void) { return m_fHeight; }
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	float GetWidth(void) { return m_fWidth; }

	static CObject3D * Create(float fHeight, float fWidth,D3DXVECTOR3 pos,D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f),int nPriority = 0);




	CObject3D * Get3D(void) { return this; }
	LPDIRECT3DTEXTURE9 * GetTex(void) { return &m_pTexture; }
	LPDIRECT3DVERTEXBUFFER9 * GetBuff(void) { return &m_pVtxBuff; }
	

protected:
	//ポリゴン表示用
	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
private:
	float m_fHeight;	//高さ
	float m_fWidth;		//幅
	
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス


};

#endif // ! _OBJECT3D_H_
