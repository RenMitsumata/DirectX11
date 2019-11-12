#pragma once

class CTexture;
class CShadow
{
private:
	float size;
	CTexture* m_Texture = NULL;
public:
	CShadow();
	CShadow(float radius);
	virtual ~CShadow();
	void Init(void);
	void Uninit(void);
	void Draw(void);
	void Draw(XMFLOAT3 pos);
};

