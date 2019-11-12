#pragma once

class CAudioClip;

class CGame : public CScene
{
private:
	CAudioClip* m_BGM;
public:
	CGame();
	virtual ~CGame();
	void Init(void);
	void Uninit(void);
	void Update(void);
};

