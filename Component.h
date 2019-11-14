////////////////////////////////////////////////////////////
//
//　Windows DirectX11プログラミング ゲーム制作
//　[Component.h]
//                                    Author:Ren Mitsumata
//==========================================================
//　GameObjectに設定する、コンポーネントの基底クラス
////////////////////////////////////////////////////////////

#ifndef _COMPONENT_H
#define _COMPONENT_H
// --- インクルードガード --- //

#include <list>

// 前方宣言
class GameObject;

class Component
{
protected:
	enum UpdatePriority {
		FIXED_UPDATE,
		NORMAL_UPDATE,
		LATE_UPDATE,
		UPDATEPRIORITY_MAX
	};

private:
	static std::list<Component*> _ComponentList[UPDATEPRIORITY_MAX];
protected:
	static void AddList(Component* component,UpdatePriority priority = NORMAL_UPDATE);
	GameObject* owner;
public:
	Component();
	Component(GameObject* owner);
	void SetOwner(GameObject* owner);
	virtual ~Component();
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	static void FixedUpdate();
	static void UpdateAll();
	static void LateUpdate();
};


// --- インクルードガード --- //
#endif