//==================
//ビルボードヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _Billboard_H_//二重インクルード防止のマクロ
#define _Billboard_H_
//インクルード
#include "main.h"
#include "object.h"

//クラス定義
class CBillboard : public CObject //オブジェクトクラスの継承
{
public:

	CBillboard(int nPriority = 5);

	~CBillboard();

	virtual	HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void SetVtx();
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	float GetHeight(void) { return m_fHeight; }
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	float GetWidth(void) { return m_fWidth; }

	static CBillboard * Create(float fHeight, float fWidth, D3DXVECTOR3 pos,char * pName = NULL);

	
	CBillboard * Get2D() { return this; }

	LPDIRECT3DVERTEXBUFFER9 * GetBuff() { return &m_pVtxBuff; }
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld; }
	void SetColor(D3DXCOLOR col) { m_Cclor = col; }
	D3DXCOLOR GetColor() { return m_Cclor; }
protected:
	//ポリゴン表示用
	
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
private:
	float m_fHeight;	//高さ
	float m_fWidth;	//幅
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス
	D3DXCOLOR m_Cclor; //色

};

#endif // ! _Billboard_H_
