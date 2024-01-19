#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
//====================================
//
//入力処理
//Author:丹野竜之介
//
//====================================
//マクロ定義
#define NUM_KEY (256) //キーの最大数
#define PLAYER_MAX (4) //プレイヤーの最大数

//入力インスタンス
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput; //本体
	LPDIRECTINPUTDEVICE8 m_pDevice;//デバイス
};
//====================================
//キーボード
class CInputKeyboard:public CInput//インプットクラスを継承
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	 HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	 void Uninit(void);
	 void Update(void);
	 bool GetPress(int nKey);
	 bool GetTrigger(int nKey);
	 bool GetRelease(int nKey);
	 bool GetRepeat(int nKey);
private:
	BYTE m_aKeyState[NUM_KEY]; //入力情報
	BYTE m_aKeyStateTrigger[NUM_KEY];//入力情報(トリガー)
	BYTE m_aKeyStateRelease[NUM_KEY];//入力情報(リリース)
	BYTE m_aKeyStateRepeat[NUM_KEY];//入力情報(リピート)
};
//====================================
//コントローラー
class CInputGamePad:public CInput
{
public:
	typedef enum
	{
		Button_UP = 0,		//上キー
		Button_DOWN,		//下キー
		Button_LEFT,		//左キー
		Button_RIGHT,		//右キー
		Button_START,		//スタート
		Button_BACK,		//バック
		Button_LStickKey,	//左トリガー押込み
		Button_RStickKey,	//右トリガー押込み
		Button_LB,			//左背部ボタン
		Button_RB,			//右背部ボタン
		Button_UNKOWN_0,	//応答なし
		Button_UNKOWN_1,	//応答なし
		Button_A,			//Aボタン
		Button_B,			//Bボタン
		Button_X,			//Xボタン
		Button_Y,			//Yボタン
		/*Button_test,*/
		Button_MAX			//ボタン最大数
	}JOYKEY;//キー番号

	CInputGamePad();
	~CInputGamePad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(JOYKEY Key, int nPlayer);
	bool GetTrigger(JOYKEY Key, int nPlayer);

	bool GetPedalL(int nPlayer);
	bool GetPedalR(int nPlayer);
	bool GetPedalTriggerL(int nPlayer);
	bool GetPedalTriggerR(int nPlayer);
	D3DXVECTOR3 GetStickL(int nPlayer, float Dead);
	D3DXVECTOR3 GetStickR(int nPlayer, float Dead);

private:
	XINPUT_STATE m_JoykeyState[PLAYER_MAX]; //キー入力情報
	XINPUT_STATE m_JoykeyStateTrigger[PLAYER_MAX];//入力情報(トリガー)
};
//====================================
//マウス
class CInputMouse:CInput
{
public:

	typedef enum
	{
		MOUSE_LEFT = 0,
		MOUSE_RIGTH,
		MOUSE_CENTER,
		MOUSE_BUTTON1,
		MOUSE_MAX
	}MOUSE;

	CInputMouse();
	~CInputMouse();


	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(MOUSE nKey);
	bool GetTrigger(MOUSE nKey) { return (m_MouseStateTrigger[nKey] & 0x80) ? true : false; }
	bool GetRelease(MOUSE nKey) { return (m_MouseStateRelease[nKey] & 0x80) ? true : false; }
	D3DXVECTOR3 GetMove(void);
private:
	LPDIRECTINPUTDEVICE8 m_pDevMouse;	//マウスのデバイス
	DIMOUSESTATE m_mouseState;
	BYTE m_MouseState[MOUSE_MAX];			//入力情報
	BYTE m_MouseStateTrigger[MOUSE_MAX];	//入力情報
	BYTE m_MouseStateRelease[MOUSE_MAX];	//入力情報
};




#endif // !_INPUT_H_