#pragma once

class CScene;

class CManager
{
private:
	static CScene* m_Scene;
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();
	static CScene* GetScene();

	template<typename T>
	static void SetScene(void){
		if (m_Scene != nullptr) {
			m_Scene->Uninit();
			delete m_Scene;
		}
		T* Scene = new T();
		
		m_Scene = Scene;
		Scene->Init();
	}
};