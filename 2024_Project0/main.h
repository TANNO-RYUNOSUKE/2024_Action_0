#ifndef  _MAIN_H_
#define _MAIN_H_
//==================
//メインヘッダー
//Author: 丹野 竜之介
//==================
//インクルード
// メモリリーク出力用マクロ
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// メモリリーク検出に必要


#include<Windows.h>
#include"d3dx9.h" //描画処理に必要

#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include "xaudio2.h"

//ライブラリにリンク
#pragma comment(lib,"d3d9.lib")//描画処理に必要
#pragma comment(lib,"d3dx9.lib")//{d3d9.lib}の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")//DirectXコンポーネントに必要
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib") //ジョイパッド用
#include "Xinput.h" //ジョイパッド用ヘッダ


//マクロ定義
#define SCREEN_WIDTH (1280)//ウィンドウ幅
#define SCREEN_HEIGHT (720)//高さ
#define SCREEN_CENTER	(D3DXVECTOR3(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT * 0.5f,0.0f))//スクリーンの中心
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#define NUM_MODEL (17)

//頂点情報の構造体を定義(2D)
typedef struct
{
	D3DXVECTOR3 pos; //頂点座標
	float rhw; //座標変換用係数(1.0f)
	D3DCOLOR col; //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
}VERTEX_2D;


//頂点情報の構造体を定義(3D)
typedef struct
{
	D3DXVECTOR3 pos; //頂点座標
	D3DXVECTOR3 nor; //法線ベクトル
	D3DCOLOR col; //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
}VERTEX_3D;

template <typename T>
T customMax(T a, T b) {
	return a > b ? a : b;
}
template <typename T>
T customMin(T a, T b) {
	return a < b ? a : b;
}
#endif // ! _MAIN_H_