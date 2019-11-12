#pragma once

struct ID3D11Buffer;
class CTexture;

class CFade
{
private:
	static ID3D11Buffer* m_VertexBuffer;	
	static bool isFade; // フェード中？
	static bool isIn;	// フェードイン？フェードアウト？(トグルスイッチ)
	static float degree;
	static CTexture* texture;
public:
	CFade();
	~CFade();
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static void Set(void);
	static bool Get(void);
};

