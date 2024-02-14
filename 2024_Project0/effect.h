//==================
//エフェクトヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _EFFECT_H_//二重インクルード防止のマクロ
#define _EFFECT_H_
//インクルード
#include "main.h"
#include "billboard.h"


//クラス定義
class CEffect : public CBillboard //オブジェクトクラスの継承
{
public:
	CEffect(int nPriority = 1);

	~CEffect();

	HRESULT Init(char * pName = "data\\TEXTURE\\effect000.jpg");
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEffect * Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife,D3DXCOLOR col,float fsize,float fAttenuation = 0.9f,float rot = 0.0f, float rotspeed = 0.0f, char * pName = "data\\TEXTURE\\effect000.jpg");


	void SetColor(D3DXCOLOR col) { m_Color = col; }
	D3DXCOLOR GetColor(void) { return m_Color; }


	void SetSize(float fSize) { m_fSize = fSize; }
	float GetSize(void) { return m_fSize; }
	void SetAttenuation(float fAttenuation) { m_fAttenuation = fAttenuation; }
	float GetAttenuation(void) { return m_fAttenuation; }
private:
	float m_fSize;//大きさ
	float m_fMaxSize;
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャへのポインタ
	D3DXVECTOR3 m_move;//移動量
	D3DXCOLOR m_Color;//色
	int m_nLife;//寿命
	int m_nMaxLife;//自分の寿命最大値
	float m_fAttenuation;//減衰率
	float Rot;
	float Rotspeed;
	char m_pName;
};

#endif // ! _EFFECT_H_