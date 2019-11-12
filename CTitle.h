#pragma once
#include "Scene.h"
class CTitle :
	public CScene
{
private:
	CPolygon* title = NULL;
	bool isChange = false;
public:
	CTitle();
	virtual ~CTitle();
	void Init(void);
	void Update(void);
};

