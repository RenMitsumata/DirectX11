
#include "manager.h"
#include "Scene.h"
#include "input.h"
#include "CTitle.h"
#include "polygon.h"
#include "CResult.h"



CResult::CResult()
{
}


CResult::~CResult()
{
}

void CResult::Init(void)
{
	resultImg = AddGameObject<CPolygon>(4);
	resultImg->Set("Assets/clear.png");
}

void CResult::Update(void)
{
	if (Input::GetKeyTrigger(VK_RETURN)) {
		CManager::SetScene<CTitle>();
	}
}
