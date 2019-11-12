#pragma once
class CColBox :	public CCollision
{
private:
	float radius;
public:
	CColBox(XMFLOAT3 pos);
	CColBox(float r, XMFLOAT3 pos);
	virtual ~CColBox();
	void Init(void);
	void Dispatch(CCollision* other);
	bool isCollision(CColSphere* other);
	bool isCollision(CColBox* other);
	float GetRadius(void) { return radius; }
	XMFLOAT3 GetPos(void) { return *pos; }
};

