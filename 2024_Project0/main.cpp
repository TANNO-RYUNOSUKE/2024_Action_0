//========================================================================================
//
// DirectX 二年目
// Author: 丹野 竜之介
//
//========================================================================================
//インクルード
#include"main.h"
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "debugproc.h"

//マクロ定義
#define CLASS_NAME "WindowClass"     //ウィンドウクラスの名前
#define WINDOW_NAME "action02"  //ウィンドウの名前
#define ID_TIMER (131)
#define TIMER_INTERVAL (1000/60)


//プロトタイプ宣言
LRESULT CALLBACK WindowPloc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=============================================
//メイン関数
//=============================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePlev, LPSTR lpCmdline, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	DWORD dwCurrentTime;
	DWORD dwExecLastTime;
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	CManager * pManager;
	CDebugProc * pDev;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,//ウィンドウスタイル
		WindowPloc, //ウィンドウプロシージャ
		0, //通常使用しない
		0, //通常使用しない
		hInstance, //インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),//マウスカーソル
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL,IDI_APPLICATION)
	};

	

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	HWND hWnd; //ウィンドウハンドル
	MSG msg;

	

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,//ウィンドウクラスの名前
		WINDOW_NAME,//ウィンドウの名前
		WS_OVERLAPPEDWINDOW,//ウィンドウスタイル
		CW_USEDEFAULT,//ウィンドウ左上のX座標
		CW_USEDEFAULT,//ウィンドウ左上のY座標
		(rect.right -rect.left),//幅
		(rect.bottom - rect.top),//高さ
		NULL,//親ウィンドウのハンドル
		NULL,//メニューハンドルまたは子ウィンドウのID
		hInstance,//インスタンスハンドル
		NULL);
	
	
	pManager = CManager::GetInstance();
	int nCountFPS = 0;

	//初期化設定
	if (FAILED(pManager->Init(hInstance,hWnd, TRUE)))
	{
		return -1;
	};
	pDev = CManager::GetInstance()->GetDeb();

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();//現在時刻を取得
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();
	srand(((unsigned int)timeGetTime()));
	ShowWindow(hWnd, nCmdShow);//ウィンドウの表示状態を設定
	DispatchMessage(&msg);
	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
			 //FPSを計測
			    nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の１秒経過
				dwExecLastTime = dwCurrentTime;
				pDev->Print("FPS:%d\n", nCountFPS);
				//更新処理
				pManager->Update();

				//描画処理
				pManager->Draw();
				dwFrameCount++; //フレームカウントを加算
			}
			

		}
	}
	if (pManager != NULL)
	{
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}
	UnregisterClass(CLASS_NAME, wcex.hInstance);



	return (int)msg.wParam;

}
//=============================================
//ウィンドウプロシージャ
//=============================================
LRESULT CALLBACK WindowPloc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
	

	switch (uMsg)
	{
	case WM_DESTROY: //ウィンドウを破棄
		PostQuitMessage(0);
		break;
	
	case WM_KEYDOWN: //キーが押された時
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウィンドウを破棄
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウィンドウを破棄
			DestroyWindow(hWnd);
		}
		else if (nID == IDNO)
		{
			return 0;
		}
		break;
	case WM_LBUTTONDOWN: //左クリック
						 //対象ウィンドウにフォーカス
		SetFocus(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

