#pragma once
class Billboard : public CGameObject
{
private:
	CTexture* texture;

public:
	Billboard();
	virtual ~Billboard();
};

