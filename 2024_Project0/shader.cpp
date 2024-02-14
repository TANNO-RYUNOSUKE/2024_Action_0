#include "shader.h"
#include "renderer.h"
#include "manager.h"
//シェーダーファイル読み込み
HRESULT Loadshader(char * path, LPD3DXEFFECT * pEffect)
{

	ID3DXBuffer* pError = NULL;
	CRenderer * pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = pRenderer->GetDevice();
	HRESULT hr = D3DXCreateEffectFromFile(
		pDevice,				// デバイスへのポインタ
		path,			// fxファイルのパス
		NULL,                  // プリプロセッサ定義
		NULL,                  // プリプロセッサ定義ファイル
		D3DXSHADER_DEBUG,                     // フラグ
		NULL,                  // カスタムエフェクトプール
		pEffect,              // 読み込まれたエフェクト
		&pError                // エラーメッセージ
	);
	if (pError)
	{
		OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
		//デバッグコンソールに表示する
		MessageBoxA(NULL, (LPCSTR)pError->GetBufferPointer(), "Shader Error", MB_OK);
	}
	return hr;
}