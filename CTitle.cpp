
#include "manager.h"
#include "CFade.h"
#include "Scene.h"
#include "input.h"
#include "CGame.h"
#include "CTitle.h"



CTitle::CTitle()
{
}


CTitle::~CTitle()
{
}

void CTitle::Init(void)
{
	title = AddGameObject<CPolygon>(4);
	title->Set("Assets/titleImage.png");
}

void CTitle::Update(void)
{
	
	if (Input::GetKeyTrigger(VK_SPACE)) {
		CFade::Set();
		isChange = true;
	}
	if (isChange && !CFade::Get()) {
		CManager::SetScene<CGame>();
	}
	
	
}

