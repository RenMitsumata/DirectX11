#pragma once
#include "Scene.h"
class CResult :
	public CScene
{
private:
	CPolygon* resultImg = NULL;
public:
	CResult();
	virtual ~CResult();
	void Init(void);
	void Update(void);
};

