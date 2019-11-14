#pragma once

class SkyDome :	public GameObject
{
private:
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	CTexture* texture = nullptr;
	int indexSize;
public:
	SkyDome();	// スカイドームの細かさを追加したかったら、int引数で追加しておいて
	virtual ~SkyDome();
	void Init(void) { return; }
	void Init(float radius);
	void Uninit(void) { return; }
	void Update(void) { return; }
	void Draw(void);
};

