#include "main.h"

#include "CCollision.h"
#include "CColSphere.h"
#include "CColBox.h"



CColBox::CColBox(XMFLOAT3 pos)
{
}

CColBox::CColBox(float r, XMFLOAT3 pos)
{
}

CColBox::~CColBox()
{
}

void CColBox::Init(void)
{
}

void CColBox::Dispatch(CCollision * other)
{
	bool is_Hit = other->isCollision(this);
	if (is_Hit && owner) {
		// “–‚½‚Á‚½‚ç‚Ç‚¤‚·‚é‚©
		//owner->SetDestroy();
	}
}

bool CColBox::isCollision(CColSphere * other)
{
	XMFLOAT3 otherPos = other->GetPos();
	float distance = (otherPos.x - this->pos->x)*(otherPos.x - this->pos->x) + (otherPos.y - this->pos->y)*(otherPos.y - this->pos->y) + (otherPos.z - this->pos->z)*(otherPos.z - this->pos->z);
	return distance < (other->GetRadius() + this->radius) * (other->GetRadius() + this->radius);
}

bool CColBox::isCollision(CColBox * other)
{
	return false;
}
