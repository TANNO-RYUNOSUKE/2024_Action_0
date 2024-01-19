//=============================================================================
//
// サウンド処理 [sound.h]
// Author :丹野竜之介
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"
//クラス定義
class CSound
{
public:

	//*****************************************************************************
	// サウンド一覧
	//*****************************************************************************
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// BGM0
		SOUND_LABEL_BGM001,			// BGM1
		SOUND_LABEL_BGM002,			// BGM2
		SOUND_LABEL_SE_SHOT,		// 弾発射音
		SOUND_LABEL_SE_EXPLOSION,	// 爆発音
		SOUND_LABEL_SE_MISSILE,
		SOUND_LABEL_SE_SLASH,
		SOUND_LABEL_SE_SLASHHIT,
		SOUND_LABEL_SE_SLASHSWING,
		SOUND_LABEL_SE_DESTRUCT,
		SOUND_LABEL_SE_HIGHSPEED,
		SOUND_LABEL_SE_LOCKON,
		SOUND_LABEL_SE_JUMP,
		SOUND_LABEL_SE_SHOCK,
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);
private:
	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice ;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apdata[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSize[SOUND_LABEL_MAX];					// オーディオデータサイズ
};



#endif
