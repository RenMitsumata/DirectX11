#include "main.h"

#include "GameObject.h"

bool CGameObject::Destroy(void) {
	if (m_Destroy) {
		Uninit();
		delete this;
		return true;
	}
	else {
		return false;
	}
}