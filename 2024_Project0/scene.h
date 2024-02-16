//==================
//シーンヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _SCENE_H_//二重インクルード防止のマクロ
#define _SCENE_H_
//インクルード
#include "main.h"
#include "object.h"
#include "object2D.h"


//前方宣言
class CEnemyManager; //エネミーマネージャー
class CInputKeyboard; //キーボード
class CInputGamePad;//コントローラー
class CInputMouse;//マウス
class CPlayer;//プレイヤー
class CEnemy;//エネミー
class CDebugProc;//デバッグ
class CExplosion;//爆発
class CRenderer;//レンダラー
class CSound;//サウンド
class CScore;//スコア
class CCamera;//カメラ
class CLight;//ライト
class CTexture;//テクスチャ
class CMeshfield;//メッシュフィールド
class CFog;//フォグ
class CFade;//フェード
class CWave;//ウェーブ
class CWave_UI;//ウェーブUI
class CEnemy_UI;//エネミーUI
class CMap_Editer;
class CRankng;



//クラス定義
class CScene :public CObject
{
public:
	CScene();
	~CScene();
	enum MODE
	{
		MODE_NONE = 0,
		MODE_TITLE,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_MAX
	};
	virtual	HRESULT Init(void) = 0;
	virtual	void Uninit(void) = 0;
	virtual	void Update(void) = 0;
	virtual	void Draw(void) = 0;

	static CScene * Create(MODE mode);
	const void SetMode(MODE mode) { m_Mode = mode; }
	MODE GetMode(void) { return m_Mode; }
	static CFade * GetFade(void) { return m_pFade; }
	static CFade * m_pFade;
	virtual CPlayer * GetPlayer(void) { return NULL; }
	virtual CCamera * GetCamera(void) { return NULL; }
	virtual int GetWave() { return -1; }
	virtual CMeshfield * GetMeshfield(void) { return NULL; }
	int m_nFrame;
private:
	MODE m_Mode;
	
};


class CTitle:public CScene
{
public:
	CTitle();
	~CTitle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	CObject2D * m_pTitle;
	D3DXVECTOR3 m_posDest;
};
class CTutorial :public CScene
{
public:
	CTutorial();
	~CTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	CObject2D * m_pTitle[2];
	int m_nDisp;
	int m_nCool;
};
class CResult :public CScene
{
public:
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
	CRankng *m_pRanking;

};

#define MAX_X (15000.0f)
#define MAX_Y (3000.0f)
#define MAX_Z (15000.0f)
class CGame :public CScene
{
public:
	CGame();
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void positionLimiter(D3DXVECTOR3 * pos);
	 CPlayer * GetPlayer(void) { return m_pPlayer; }
	 CMeshfield * GetMeshfield(void) { return m_pMeshfield; }
	 CCamera * GetCamera(void) { return m_pCamera; }

	 int GetWave() { return m_nWaveCount; }
	 void SetPlayer(CPlayer * pPlayer) { m_pPlayer = pPlayer; }
private:
	
	 CPlayer * m_pPlayer;//プレイヤーのポインタ
	
	 CCamera * m_pCamera;//カメラのポインタ
	 CLight * m_pLight;//ライトのポインタ
	 CMeshfield * m_pMeshfield;//メッシュフィールドへのポインタ
	 int m_nEnemy;
	int m_nCnt;
	int m_nEnemyCount;
	int m_nWaveCount;
	bool b_Pause;//ポーズ
};



#endif // ! _SCENE_H_
