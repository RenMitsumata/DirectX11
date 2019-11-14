#pragma once
class Billboard : public GameObject
{
private:
	CTexture* texture;

public:
	Billboard();
	virtual ~Billboard();
};

