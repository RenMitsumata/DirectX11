#pragma once

#define FOCUS_LENGTH (5)

class CCamera : public CGameObject
{
private:
	
	
	RECT m_Viewport;

	XMMATRIX worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projMatrix;

	XMVECTOR eye;
	XMVECTOR at;
	XMVECTOR up;
	int enemyNum;

	float    fov;
	float    aspect;
	float    nearZ;
	float    farZ;

public:
	void Init();
	void Uninit();
	void Update();
	void Update(XMFLOAT3 newPos);
	void Update(XMFLOAT3 newPos,XMVECTOR frontVec,XMVECTOR upVec);
	void Draw();

	bool GetVisiblity(XMFLOAT3 position);

};