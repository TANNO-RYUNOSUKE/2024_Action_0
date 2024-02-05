//==================
//カメラヘッダー
//Author: 丹野 竜之介
//==================
#include "main.h"

#ifndef _Camera_H_
#define _Camera_H_

class CCamera
{
public:
	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);


	void SetRDest(D3DXVECTOR3 pos) { m_posRdest = pos; }
	D3DXVECTOR3 GetRDest(void) { return m_posRdest; }
	void SetVDest(D3DXVECTOR3 pos) { m_posVdest = pos; }
	void SetShake(int nCnt) { nCntShake = nCnt; }
	int GetShake(void) { return nCntShake; }
	void SetLenght(float fLength) { m_fLengthCamera = fLength; }
	float GetLenght(void) { return m_fLengthCamera; }
	void SetDirection(bool bDirection) { m_bDirection = bDirection; }
	D3DXVECTOR3 GetRot() { return m_rot; }//向きの取得
	D3DXVECTOR3 GetRotDest() { return m_rotDest; }//向きの取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetRotDest(D3DXVECTOR3 rot) { m_rotDest = rot; }
	D3DXVECTOR3 GetVec(void) { return m_vecCamera; }
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリクス
	D3DXMATRIX m_mtxView;		//ビューマトリクス
private:
	D3DXVECTOR3 m_posV;			//視点
	D3DXVECTOR3 m_posR;			//注視点
	D3DXVECTOR3 m_posVdest;		//視点の目的地
	D3DXVECTOR3 m_posRdest;		//注視点の目的地
	D3DXVECTOR3 m_vecU;			//上方向ベクトル
	D3DXVECTOR3 m_vecCamera;	//カメラの向いている方向
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//向き目標
	D3DXVECTOR3 m_moveV;		//視点の移動速度
	D3DXVECTOR3 m_moveR;		//注視点の移動速度
	
	float m_fLengthCamera; //カメラの距離
	D3DXVECTOR3 m_rotOld;
	int nCntShake;
	bool m_bDirection;
};



#endif // !_Camera_H_
