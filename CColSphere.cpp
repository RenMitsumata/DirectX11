#include "main.h"
#include "manager.h"
#include "Scene.h"
#include "CCollision.h"

#include "CColSphere.h"




CColSphere::~CColSphere()
{
}


void CColSphere::Dispatch(CCollision * other)
{
	bool is_Hit = other->isCollision(this);
	if (is_Hit && owner) {
		// “–‚½‚Á‚½‚ç‚Ç‚¤‚·‚é‚©
		this->SetDestroy();
		other->SetDestroy();
	}
}

bool CColSphere::isCollision(CColSphere * other)
{
	XMFLOAT3 otherPos = other->GetPos();
	float distance = (otherPos.x - this->pos->x)*(otherPos.x - this->pos->x) + (otherPos.y - this->pos->y)*(otherPos.y - this->pos->y) + (otherPos.z - this->pos->z)*(otherPos.z - this->pos->z);
	return distance < (other->GetRadius() * other->GetRadius()) + (this->radius * this->radius);
}

bool CColSphere::isCollision(CColBox * other)
{
	return false;
}

