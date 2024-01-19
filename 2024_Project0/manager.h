//==================
//マネージャヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _MANAGER_H_//二重インクルード防止のマクロ
#define _MANAGER_H_
//インクルード
#include "main.h"
#include "scene.h"

#define VECTO3ZERO (D3DXVECTOR3(0.0f,0.0f,0.0f))

//前方宣言
class CEnemyManager; //エネミーマネージャー[
class CItemManager; //エネミーマネージャー
class CXManager; //Xマネージャー
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
class CXFile;//Xファイル
class CMeshfield;//メッシュフィールド
class CFog;//フォグ
class CScene;


//クラス定義
class CManager //マネージャクラス
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hinstance,HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPause(bool bPause) { m_bPause = bPause; }
	void SetHitStop(int nStop) { m_nHitStop = nStop; }
	int GetHitStop(void) { return m_nHitStop; }
	void SetScore(int nScore) { m_nScoreSave = nScore; }
	int GetScore(void) { return m_nScoreSave; }
	bool GetPause(void) { return m_bPause; }
	void SetMode(CScene::MODE mode);
	CScene * GetMode(void);

	void ReSetItem(void);
	float GetDistance(D3DXVECTOR3 vec);
	bool RayTri(const D3DXVECTOR3& startPoint, const D3DXVECTOR3& endPoint, const D3DXVECTOR3& polypos1, const D3DXVECTOR3& polypos2, const D3DXVECTOR3& polypos3,D3DXVECTOR3 * pPos = NULL);

	CInputKeyboard * GetInputKeyboard(void);
	CInputGamePad * GetInputGamePad(void);
	CInputMouse * GetInputMouse(void);
	CRenderer * GetRenderer(void);

	CDebugProc * GetDeb(void);
	CSound * GetSound(void) { return m_pSound; }
	CTexture * GetTexture(void) { return m_pTexture; }
	CXFile * GetXFiles(void) { return m_pXFile; }
	CEnemyManager * GetEnemyManager(void) { return m_pEnemyManager; }
	CItemManager * GetItemManager(void) { return m_pItemManager; }
	CXManager * GetXManager(void) { return m_pXManager; }
	CScene * GetScene(void){ return m_pScene; }

	static CManager * GetInstance() {
		if (pInstance == NULL) { pInstance = DBG_NEW CManager; return pInstance; }
		else
		{
			return pInstance;
		}
	}
private:
	static CManager * pInstance;
	 CRenderer * m_pRenderer; //レンダラーのポインタ
	 CInputKeyboard * m_pInputKeyboard;//キーボードのポインタ
	 CInputGamePad * m_pInputGamepad;//キーボードのポインタ
	 CInputMouse * m_pInputMouse;//キーボードのポインタ
	 CDebugProc * m_pDeb;//デバッグ表示のポインタ
	 CSound * m_pSound;//サウンドポインタ
	 CTexture * m_pTexture;//テクスチャのポインタ
	 CXFile * m_pXFile;
	 CEnemyManager * m_pEnemyManager; //エネミーマネージャのポインタ
	 CItemManager * m_pItemManager; //アイテムマネージャのポインタ
	 CXManager * m_pXManager; //エネミーマネージャのポインタ
	 CScene * m_pScene;//シーン
	 bool m_bPause;
	 int m_nHitStop;
	 int m_nScoreSave;
};


#endif // ! _MANAGER_H_
