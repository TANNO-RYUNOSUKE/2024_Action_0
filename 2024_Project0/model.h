//==================
//モデルヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _MODEL_H_//二重インクルード防止のマクロ
#define _MODEL_H_
//インクルード
#include "main.h"
#include "object.h"
#include<string.h>
#include<string>
//クラス定義
class CModel 
{
public:

	CModel();

	~CModel();

	virtual	HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);


	static CModel * Create(char * pModelName);
	void SetParent(CModel * pModel) { m_pParent = pModel; }
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetMin(void) { return m_vtxMinModel; }
	D3DXVECTOR3 GetMax(void) { return m_vtxMaxModel; }
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld; }
	D3DXMATRIX * GetMatrixAddress(void) { return &m_mtxWorld; }
	std::string GetName(void) { return m_pModelName; }
	void SetOffset(D3DXVECTOR3 pos) { m_offset = pos; }
	D3DXVECTOR3 GetOffset() { return m_offset; }
	void SetCol(D3DXCOLOR col) { m_Col = col; }
	int * GetAddles() { return m_pIdxTex; }
protected:
	int * m_pIdxTex;//テクスチャのアドレスを保持するポインタ
	int  m_nIdxXFile;//モデルのアドレス
	std::string m_pModelName;//読み込むモデルの名前
	D3DXCOLOR m_Col;
private:
	
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
	LPD3DXMESH m_pMesh;		//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;	//マテリアルへのポインタ
	DWORD m_dwNumMat;			//マテリアルの数
	D3DXVECTOR3 m_vtxMinModel, m_vtxMaxModel;//モデルの最少と最大

	D3DXVECTOR3 m_pos;//座標
	D3DXVECTOR3 m_rot;//角度
	D3DXVECTOR3 m_offset;
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス
	
	CModel * m_pParent;//親のモデル

};

class CAfterImage :public CModel
{
public:

	CAfterImage();

	~CAfterImage();

	HRESULT Init(void);
	 void Uninit(void);
	 void Update(void);
	 void Draw(void);

	 int GetLife() { return m_nLife; }
	 static CAfterImage * Create(char * pModelName,D3DXMATRIX mtx,D3DXCOLOR col, int nLife);
	
private:
	int m_nLife;
	D3DXMATRIX m_Mtx;
};
class CAfterImageObject :public CObject
{
public:
	CAfterImageObject();

	~CAfterImageObject();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);


	static CAfterImageObject * Create(char * pModelName, D3DXMATRIX mtx, D3DXCOLOR col, int nLife);

private:
	CAfterImage * m_pAfterImage;
};
#endif // ! _MODEL_H_
