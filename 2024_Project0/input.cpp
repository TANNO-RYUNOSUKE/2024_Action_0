//========================================================================================
//
// 入力処理
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include "input.h"
//==============================
//静的メンバ
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//==============================
//コンストラクタ、デストラクタ
//==============================
CInput::CInput()
{
	m_pDevice = NULL;
}

CInput::~CInput()
{
}

//==============================
//初期化処理
//==============================
HRESULT CInput:: Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		//DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	
	return S_OK;
}
//======================================
//終了処理
//======================================
void  CInput::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire(); //アクセス権を破棄
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//==============================
//コンストラクタ、デストラクタ
//==============================
CInputKeyboard::CInputKeyboard()
{
	m_aKeyState[0] = {};
	m_aKeyStateRelease[0] = {};
	m_aKeyStateTrigger[0] = {};
	m_aKeyStateRepeat[0] = {};
}

CInputKeyboard::~CInputKeyboard()
{
}
//======================================
//キーボード初期化
//======================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	//入力デバイスの生成（キーボード）
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//キーボードのアクセス権を取得
	m_pDevice->Acquire();

	return S_OK;
}
//======================================
//キーボード終了
//======================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}
//======================================
//キーボード更新
//======================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY];
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&m_aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey]; //キーボードのPRESS情報を保存
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//======================================
//プレス情報
//======================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0X80) ? true : false;
}
//======================================
//トリガー情報
//======================================
bool  CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0X80) ? true : false;
}
//======================================
//リリース情報
//======================================
bool  CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0X80) ? true : false;
}
//======================================
//リピート情報
//======================================
bool  CInputKeyboard::GetRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0X80) ? true : false;
}
//==============================
//コンストラクタ、デストラクタ
//==============================
CInputGamePad::CInputGamePad()
{
	m_JoykeyState[0] = {};
	m_JoykeyStateTrigger[0] = {};
}

CInputGamePad::~CInputGamePad()
{
}
//======================================
//ゲームパッド初期化
//======================================
HRESULT CInputGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//Xinputのステートの設定
	XInputEnable(true);

	//ゼロクリア
	ZeroMemory(&m_JoykeyState, sizeof(m_JoykeyState));
	ZeroMemory(&m_JoykeyStateTrigger, sizeof(m_JoykeyStateTrigger));

	return S_OK;
}
//======================================
//ゲームパッド終了
//======================================
void CInputGamePad::Uninit(void)
{
	//Xinputのステートの設定
	XInputEnable(false);
}
//======================================
//ゲームパッド更新
//======================================
void CInputGamePad::Update(void)
{
	int nCount;
	XINPUT_STATE JoykeyState;

	for (nCount = 0; nCount < PLAYER_MAX; nCount++)
	{
		if (XInputGetState(nCount, &JoykeyState) == ERROR_SUCCESS)
		{
			m_JoykeyStateTrigger[nCount].Gamepad.wButtons = (m_JoykeyState[nCount].Gamepad.wButtons ^ JoykeyState.Gamepad.wButtons) & JoykeyState.Gamepad.wButtons;
			m_JoykeyStateTrigger[nCount].Gamepad.bLeftTrigger = (m_JoykeyState[nCount].Gamepad.bLeftTrigger ^ JoykeyState.Gamepad.bLeftTrigger) & JoykeyState.Gamepad.bLeftTrigger;
			m_JoykeyStateTrigger[nCount].Gamepad.bRightTrigger = (m_JoykeyState[nCount].Gamepad.bRightTrigger ^ JoykeyState.Gamepad.bRightTrigger) & JoykeyState.Gamepad.bRightTrigger;

			m_JoykeyState[nCount] = JoykeyState;
		}
	}

}
//======================================
//プレス情報
//======================================
bool CInputGamePad::GetPress(JOYKEY Key, int nPlayer)
{
	return (m_JoykeyState[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}
//======================================
//トリガー情報
//======================================
bool CInputGamePad::GetTrigger(JOYKEY Key, int nPlayer)
{
	return (m_JoykeyStateTrigger[nPlayer].Gamepad.wButtons & (0x01 << Key)) ? true : false;
}
//======================================
//ペダルプレス情報L
//======================================
bool CInputGamePad::GetPedalL(int nPlayer)
{
	return (m_JoykeyState[nPlayer].Gamepad.bLeftTrigger & (0x01 << '\0')) ? true : false;
}
//======================================
//ペダルプレス情報R
//======================================
bool CInputGamePad::GetPedalR(int nPlayer)
{
	return (m_JoykeyState[nPlayer].Gamepad.bRightTrigger & (0x01 << '\0')) ? true : false;
}
//======================================
//ペダルトリガー情報L
//======================================
bool CInputGamePad::GetPedalTriggerL(int nPlayer)
{
	return (m_JoykeyStateTrigger[nPlayer].Gamepad.bLeftTrigger & (0x01 << '\0')) ? true : false;
}
//======================================
//ペダルトリガー情報R
//======================================
bool CInputGamePad::GetPedalTriggerR(int nPlayer)
{
	return (m_JoykeyStateTrigger[nPlayer].Gamepad.bRightTrigger & (0x01 << '\0')) ? true : false;
}
//======================================
//スティック情報L
//======================================
D3DXVECTOR3 CInputGamePad::GetStickL(int nPlayer, float Dead)
{
	D3DXVECTOR3 Stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Stick.x = (float)m_JoykeyState[nPlayer].Gamepad.sThumbLX / SHRT_MAX;
	if (fabsf(Stick.x) < Dead)
	{
		Stick.x = 0.0f;
	}

	Stick.y = (float)m_JoykeyState[nPlayer].Gamepad.sThumbLY / SHRT_MAX;
	if (fabsf(Stick.y) < Dead)
	{
		Stick.y = 0.0f;
	}
	Stick.y *= -1;

	return Stick;
}
//======================================
//スティック情報R
//======================================
D3DXVECTOR3 CInputGamePad::GetStickR(int nPlayer, float Dead)
{
	D3DXVECTOR3 Stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Stick.x = (float)m_JoykeyState[nPlayer].Gamepad.sThumbRX / (SHRT_MAX);
	if (fabsf(Stick.x) < Dead)
	{
		Stick.x = 0.0f;
	}

	Stick.y = (float)m_JoykeyState[nPlayer].Gamepad.sThumbRY / (SHRT_MAX);
	if (fabsf(Stick.y) < Dead)
	{
		Stick.y = 0.0f;
	}
	Stick.y *= -1;

	return Stick;
}
//==============================
//コンストラクタ、デストラクタ
//==============================
CInputMouse::CInputMouse()
{
	m_pDevMouse = NULL;

}

CInputMouse::~CInputMouse()
{
}

//======================================
//マウス初期化
//======================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//入力デバイスの生成（マウス）
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevMouse, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(m_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED(m_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(m_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	//マウスのアクセス権を取得
	m_pDevMouse->Acquire();

	return S_OK;
}
//======================================
//マウス終了
//======================================
void CInputMouse::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevMouse != NULL)
	{
		m_pDevMouse->Unacquire(); //キーボードへのアクセス権を破棄
		m_pDevMouse->Release();
		m_pDevMouse = NULL;
	}

	//Directinputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}
//======================================
//マウス更新
//======================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE mouse;
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevMouse->GetDeviceState(sizeof(mouse), &mouse)))
	{
		for (nCntKey = 0; nCntKey < CInputMouse::MOUSE_MAX; nCntKey++)
		{
			//トリガー情報
			m_MouseStateTrigger[nCntKey] = (m_mouseState.rgbButtons[nCntKey] ^ mouse.rgbButtons[nCntKey])
				& mouse.rgbButtons[nCntKey];
			m_MouseStateRelease[nCntKey] = (m_mouseState.rgbButtons[nCntKey] ^ mouse.rgbButtons[nCntKey])
				& m_mouseState.rgbButtons[nCntKey];
			//入力情報の保存
			m_mouseState.rgbButtons[nCntKey] = mouse.rgbButtons[nCntKey];
			m_mouseState.lX = mouse.lX;
			m_mouseState.lY = mouse.lY;
			m_mouseState.lZ = mouse.lZ;
		}
	
	}
	else
	{
		m_pDevMouse->Acquire();
	}
}
//=============================
//マウスの入力
//=============================
bool CInputMouse::GetPress(MOUSE nKey)
{
	return (m_mouseState.rgbButtons[nKey] & 0x80) ? true : false;
}
//=============================
//マウスの移動取得
//=============================
D3DXVECTOR3 CInputMouse::GetMove(void)
{
	return D3DXVECTOR3(((float)m_mouseState.lX), (-(float)m_mouseState.lY), (-(float)m_mouseState.lZ));
}