//================================
//ライトヘッダ
//================================
#include "main.h"

#ifndef _Light_H_
#define _Light_H_

#define NUM_LIGHT (3)
class CLight
{
public:
	CLight();
	~CLight();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
private:
	D3DLIGHT9 m_aLight[NUM_LIGHT];//保持するライトのデータ
};


#endif // !_Light_H_