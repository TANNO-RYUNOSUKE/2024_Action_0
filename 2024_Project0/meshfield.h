//==================
//メッシュフィールドヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _MESHFIELD_H_//二重インクルード防止のマクロ
#define _MESHFIELD_H_
//インクルード
#include "main.h"
#include "object.h"

//クラス定義
class CMeshfield : public CObject //オブジェクトクラスの継承
{
public:

	CMeshfield(int nPriority = 0);

	~CMeshfield();

	virtual	HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	float GetHeight(void) { return m_fHeight; }
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	float GetWidth(void) { return m_fWidth; }

	static CMeshfield * Create(float fHeight, float fWidth, int nU,int nV,D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	bool MeshRay(const D3DXVECTOR3 & Start, const D3DXVECTOR3 & End);
	bool Collision(D3DXVECTOR3 * ppos);
	void Save(void);
	void Load(void);
	CMeshfield * Get3D(void) { return this; }
	LPDIRECT3DTEXTURE9 * GetTex(void) { return &m_pTexture; }
	LPDIRECT3DVERTEXBUFFER9 * GetBuff(void) { return &m_pVtxBuff; }

private:
	//ポリゴン表示用
	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
	PDIRECT3DINDEXBUFFER9 m_pIdxBuff; //インデックスバッファ
	float m_fHeight;	//高さ
	float m_fWidth;		//幅
	int m_nSplit_V;//縦の分割数
	int m_nSplit_U;//横の分割数
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス


};

#endif // ! _MESHFIELD_H_
