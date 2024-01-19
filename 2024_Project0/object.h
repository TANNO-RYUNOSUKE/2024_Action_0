//==================
//オブジェクトヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _OBJECT_H_//二重インクルード防止のマクロ
#define _OBJECT_H_
//インクルード
#include "main.h"

//マクロ定義
#define NUM_OBJECT (2560) //オブジェクトの最大数
#define NUM_PRIORITY (8) //優先順位の最大

//クラス定義
class CObject //オブジェクトの抽象クラス
{
public:
	typedef enum
	{
		TYPE_NONE = 0,//空のタイプ
		TYPE_ENEMY,//敵
		TYPE_PLAYER,//プレイヤー
		TYPE_BULLET,//弾
		TYPE_BLOCK,//ブロック
		TYPE_ITEM,//アイテム
		TYPE_MAX,//最大値
	}TYPE;//タイプ

	CObject(int nPriority = 3);
	virtual ~CObject();

	virtual HRESULT Init(void) = 0;

	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	
	static void ReleaseAll(void);
	static void DrawAll(void);
	static void UpDateAll(void);

	D3DXVECTOR3 GetPos();// { return m_pos; }//座標取得
	D3DXVECTOR3 * GetPosAddress() { return  &m_pos; }//座標取得
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }//座標設定
	D3DXVECTOR3 GetRot() { return m_rot; }//角度取得
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }//角度設定
	D3DXVECTOR3 GetMove() { return m_move; }//移動取得
	D3DXVECTOR3 * GetMoveAddress() { return &m_move; }//移動取得
	void SetMove(const D3DXVECTOR3 move) {  m_move = move; }//移動設定

	void SetType(const TYPE type) { m_Type = type; }//タイプの設定
	TYPE GetType(void) { return m_Type; }//タイプの取得

	virtual D3DXVECTOR3 GetPosOld() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	virtual void SetLand(bool bLand) {};

	virtual float GetHeight(void) { return 0.0f; }
	virtual float GetWidth(void) { return 0.0f; }
	virtual void SetLife(int nLife) { };
	virtual int GetLife() { return 0; }
	virtual D3DXMATRIX GetMatrix(void) {D3DXMATRIX mtx;  D3DXMatrixIdentity(&mtx); return mtx;}
	//int GetID(void) { return m_nID; }
	
	static CObject * GetTop(int nPriority) {
		if (nPriority >= 0 && nPriority < NUM_PRIORITY) { return m_apTop[nPriority]; }
		else
		{
			return NULL;
		}
	}
	CObject * GetNext(){ return m_pNext; }
	void SetNext(CObject * pNext) { m_pNext = pNext; }
	CObject * GetPrev() { return m_pPrev; }
	void SetPrev(CObject * pPrev) { m_pPrev = pPrev; }
	
	virtual void Release(void);

	virtual void SetMtx(D3DXMATRIX mtx) { m_mtx = mtx; }
	virtual D3DXMATRIX GetMtx() { return m_mtx; }
protected:
	int m_nIdxTex[8];//テクスチャのアドレスを保持
	D3DXMATRIX m_mtx;
private:
	//static CObject * m_apObject[NUM_PRIORITY][NUM_OBJECT];//オブジェクトのポインタ
	static CObject * m_apTop[NUM_PRIORITY];//オブジェクトの先頭
	static CObject * m_apCur[NUM_PRIORITY];//オブジェクトの最後尾
	static CObject * m_pStaticNext;//次のオブジェクト
	CObject * m_pPrev;//自分の前のポインタ
	CObject * m_pNext;//自分の次のポインタ
	static int m_nNumALL;//合計数
	
	int m_nPriority;//オブジェクトの優先順位
	D3DXVECTOR3 m_pos; //座標
	D3DXVECTOR3 m_move; //移動速度
	D3DXVECTOR3 m_rot; //向き
	TYPE m_Type; //タイプ
	
};


#endif // ! _OBJECT_H_
