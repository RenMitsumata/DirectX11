#pragma once

class SkyDome :	public CGameObject
{
private:
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	CTexture* texture = nullptr;
	int indexSize;
public:
	SkyDome();	// �X�J�C�h�[���ׂ̍�����ǉ�������������Aint�����Œǉ����Ă�����
	virtual ~SkyDome();
	void Init(void) { return; }
	void Init(float radius);
	void Uninit(void) { return; }
	void Update(void) { return; }
	void Draw(void);
};

