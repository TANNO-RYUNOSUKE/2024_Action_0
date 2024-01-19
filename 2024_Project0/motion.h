//==================
//モーションヘッダー
//Author: 丹野 竜之介
//==================
#ifndef  _MOTION_H_//二重インクルード防止のマクロ
#define _MOTION_H_
//インクルード
#include "main.h"
#include "model.h"

#define NUM_KEY_MOTION (24)//キーの総数
#define NUM_KEYINFO (16)//インフォの総数
#define NUM_INFO (16)//モーションの総数
//クラス定義
class CMotion
{
public:

	CMotion();
	~CMotion();

	typedef struct
	{
		float pos_x;
		float pos_y;
		float pos_z;
		float rot_x;
		float rot_y;
		float rot_z;

	}KEY;//座標と角度

	typedef struct
	{
		int nFrame;//再生時間
		KEY aKey[NUM_KEY_MOTION];//キーの配列
	}KEY_INFO;//キーの情報


	typedef struct
	{
		bool bLoop;
		int nNumkey;//再生時間
		KEY_INFO aKeyInfo[NUM_KEYINFO];//キーの配列
	}INFO;

	void Set(int nType) { m_nType = nType; }
	void Update(void);
	void Uninit(void);
	int GetType(void) { return m_nType; }
	void SetType(int nType) {
		if (m_nType != nType) {
			m_nType = nType; m_nkey = 0; m_nCounter = 0; m_bBlend = true;
			for (int i = 0; i < m_nNumModel; i++)
			{
				D3DXVECTOR3 pos = (*m_ppModel[i])->GetPos();
				D3DXVECTOR3 rot = (*m_ppModel[i])->GetRot();
			
				m_Prevkey[i].rot_x = rot.x;
				m_Prevkey[i].rot_y = rot.y;
				m_Prevkey[i].rot_z = rot.z;
			}
		}
	}
	void SetkeyNumber(int nkey) { m_nkey = nkey; }
	bool IsFinish(void) { return m_bFinish; }
	void SetInfo(INFO Info);
	void SetModel(CModel**ppModel) { 
		for (int nCnt = 0; nCnt < NUM_MODEL; nCnt++)
		{
			m_ppModel[nCnt] = ppModel + nCnt;
		}	
	}
	void SetNumModel(int nNum){ m_nNumModel = nNum; }
	int GetNumModel(void) { return m_nNumModel; }

	int GetKey(void) { return m_nkey; }
	void SetLoop(bool bLoop) { m_bLoop = m_bLoop; }
	void SetNumKey(int nInfo, int nNumKey) { m_Info[nInfo].nNumkey = nNumKey; }
	int GetNumKey(int nInfo) { return m_Info[nInfo].nNumkey; }
	void SetFrame(int nInfo, int nKey_info,int nFrame) { m_Info[nInfo].aKeyInfo[nKey_info].nFrame = nFrame; }
	void SetKey(int nInfo, int nKey_info, int nKey, KEY key) { m_Info[nInfo].aKeyInfo[nKey_info].aKey[nKey] = key; }
	void Load(char * cFile);


	
private:
	INFO m_Info[NUM_INFO];
	KEY m_Prevkey[NUM_KEY_MOTION];
	int m_nNumAll;
	int m_nType;
	bool m_bLoop;
	int m_nkey;
	int m_nCounter;
	bool m_bFinish;
	CModel **m_ppModel[NUM_MODEL];
	CModel * m_ppModelList[NUM_MODEL];
	int m_nNumModel;
	bool m_bBlend;
};


#endif // ! _MOTION_H_
