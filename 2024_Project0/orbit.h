//==================
//3Dオブジェクトヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _Orbit_H_//二重インクルード防止のマクロ
#define _Orbit_H_
//インクルード
#include "main.h"
#include "object.h"

//クラス定義
class COrbit : public CObject //オブジェクトクラスの継承
{
public:

	COrbit(int nPriority = 6);

	~COrbit();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static COrbit * Create(int nLength, D3DXCOLOR col,D3DXVECTOR3 Offset1, D3DXVECTOR3 Offset2, D3DXMATRIX * pMtx,int nPriority = 4);


	void end(void) { m_bEnd = true; }
	void SetDisp(bool bDisp) { m_bDisp = bDisp; }
	COrbit * Get3D(void) { return this; }
	LPDIRECT3DTEXTURE9 * GetTex(void) { return &m_pTexture; }
	LPDIRECT3DVERTEXBUFFER9 * GetBuff(void) { return &m_pVtxBuff; }
	void SetOffset(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2) { m_Offset1 = pos1; m_Offset2 = pos2; }

protected:
	//ポリゴン表示用
	LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
private:

	int m_nLength;//長さ
	D3DXVECTOR3 * m_pPosOld;//座標記録用
	D3DXVECTOR3 m_Offset1;//座標
	D3DXVECTOR3 m_Offset2;//座標
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス
	D3DXMATRIX * m_pMtx; 
	int m_nTex;
	D3DXCOLOR m_col;
	bool m_bEnd;
	bool m_bDisp;
	int m_nLife;
};

#endif // ! _Orbit_H_
