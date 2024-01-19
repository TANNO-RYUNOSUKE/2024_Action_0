//==================
//Xオブジェクトヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _OBJECTX_H_//二重インクルード防止のマクロ
#define _OBJECTX_H_
//インクルード
#include "main.h"
#include "object.h"
#include <string>
#include <algorithm>
#include<vector>
//クラス定義
class CObjectX : public CObject //オブジェクトクラスの継承
{
public:

	CObjectX(int nPriority = 3);

	~CObjectX();

	virtual	HRESULT Init();
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	bool Ray(D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXVECTOR3 * pPos = NULL);
	bool Collision(D3DXVECTOR3 prevPosition, D3DXVECTOR3* pCurrentPosition, D3DXVECTOR3* pMovement);
	bool IntersectTriangle(const D3DXVECTOR3& prevPosition, D3DXVECTOR3* pCurrentPosition, D3DXVECTOR3* pMovement, const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3);
	bool IsInsideBoundingBox(D3DXVECTOR3* vertices, D3DXVECTOR3 point, D3DXVECTOR3* pMovement);
	bool IsOnLineSegment(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 point);
	D3DXVECTOR3 CalculateNormal(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2, const D3DXVECTOR3& v3);
	static CObjectX * Create(char * pName, D3DXVECTOR3 pos, D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f), int nPriority = 3);
    void CorrectMovementForPlane(const D3DXVECTOR3& prevPosition, D3DXVECTOR3* pCurrentPosition, D3DXVECTOR3* pMovement, const D3DXVECTOR3& point1, const D3DXVECTOR3& point2, const D3DXVECTOR3& point3);
	void SetID(int nId) { m_nID = nId; }
	int GetID(void) { return m_nID; }

	void SetName(std::string name) { m_pModelName = name; }
	std::string GetName(void) { return m_pModelName; }
protected:
	//ポリゴン表示用
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス
	int m_nID;
	int m_nIdxXFile;
private:
	std::string m_pModelName;//読み込むモデルの名前
	
	D3DXVECTOR3 m_vtxMinModel, m_vtxMaxModel;//モデルの最少と最大

	

};

class CDebri : public CObjectX //オブジェクトクラスの継承
{
public:
	CDebri(int nPriority = 3);
	~CDebri();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);


	static CDebri * Create(std::string pName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 rotation, int nLife, int nPriority = 3);
private:
	int m_nLife;
	D3DXVECTOR3 move, rotation;
};


#endif // ! _OBJECTX_H_
