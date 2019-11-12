
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
	resultImg->Set("asset/clear.png");
}

void CResult::Update(void)
{
	if (CInput::GetKeyTrigger(VK_RETURN)) {
		CManager::SetScene<CTitle>();
	}
}
