//==================
//2Dオブジェクトヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _OBJECT2D_H_//二重インクルード防止のマクロ
#define _OBJECT2D_H_
//インクルード
#include "main.h"
#include "object.h"

//クラス定義
class CObject2D : public CObject //オブジェクトクラスの継承
{
public:

	CObject2D(int nPriority = 6);

	~CObject2D();

    virtual	HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	float GetHeight(void) { return m_fHeight; }
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	float GetWidth(void) { return m_fWidth; }

	static CObject2D * Create(D3DXVECTOR3 pos, float fHeight, float fWidth,int nPriority = 0,char * pFileName = NULL);

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth);
	void SetVtx(VERTEX_2D  * pVtx);

	CObject2D * Get2D() { return this; }
	LPDIRECT3DTEXTURE9 * GetTex() { return &m_pTexture; }
	LPDIRECT3DVERTEXBUFFER9 * GetBuff() { return &m_pVtxBuff; }

	D3DXCOLOR GetCol(void) { return m_col; }
	void SetCol(D3DXCOLOR col) { m_col = col; }
	void SetDisp(bool bDisp) { m_bDisp = bDisp; }
	void SetLife(int nLife) { m_nLife = nLife; }
protected:
	//ポリゴン表示用
	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
private:
	float m_fHeight;	//高さ
	float m_fWidth;	//幅
	D3DXCOLOR m_col;//色
	char * m_pFileName;//名前
	bool m_bDisp;
	int m_nLife;
};

#endif // ! _OBJECT2D_H_
