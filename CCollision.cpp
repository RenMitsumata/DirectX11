#include "main.h"
#include "GameObject.h"
#include "CCollision.h"


CCollision::CCollision()
{
}


CCollision::~CCollision()
{
}

bool CCollision::Destroy(void)
{
	
	if (m_Destroy) {
		if (owner) {
			owner->SetDestroy();
		}
		delete this;
		return true;
	}
	else {
		return false;
	}
}


