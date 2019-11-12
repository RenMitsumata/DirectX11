#pragma once

//class CTexture;
class CField : public CGameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;
	CTexture* m_Texture = NULL;
	CTexture* m_WallTexture = NULL;
	static VERTEX_3D* vertex;
public:
	CField();
	~CField();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	static float GetHeight(XMFLOAT3 pos);
	static XMFLOAT4 GetNormal(XMFLOAT3* pos);
};


