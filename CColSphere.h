#pragma once
class CColSphere :	public CCollision
{
private:
	float radius;
public:
	CColSphere() { radius = 1; }
	virtual ~CColSphere();
	void SetRadius(float radius) { this->radius = radius; }
	void Dispatch(CCollision* other);
	bool isCollision(CColSphere* other);
	bool isCollision(CColBox* other);
	float GetRadius(void) { return radius; }
	XMFLOAT3 GetPos(void) { return *pos; }
};

