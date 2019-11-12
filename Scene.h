#pragma once


#include <list>
#include <vector>
#include <typeinfo>
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "GameObject.h"
#include "CCollision.h"
#include "CPlayer.h"
#include "Enemy.h"
#include "camera.h"
#include "Field.h"
#include "polygon.h"
#include "SkyDome.h"
#include "CBullet.h"
#include "Cource.h"

class CScene
{
protected:
	std::list<CGameObject*> _Objectlist[5];
	std::list<CCollision*> _Collisionlist;
public:
	
	template<typename T>
	T* AddGameObject(int layer) {
		T* object = new T();
		object->Init();
		_Objectlist[layer].push_back(object);
		return object;
	}

	template<typename TC>
	TC* AddCollision(void){
		TC* collision = new TC();
		_Collisionlist.push_back(collision);
		return collision;
	}
	
	template<typename T>
	T* GetGameObject(int Layer) {
		for(CGameObject* object : _Objectlist[Layer]) {
			if (typeid(*object) == typeid(T)) {
				return (T*)object;
			}
		}
	}
	
	
	template<typename T>
	std::vector<T*> GetGameObjects(int Layer) {
		std::vector<T*> objects;
		for (CGameObject* object : _Objectlist[Layer]) {
			if (typeid(*object) == typeid(T)) {
				objects.push_back((T*)object);
			}
		}
		return objects;
	}
	
	CScene() {}
	virtual ~CScene() {}
	virtual void Init(void) = 0;
	
	virtual void Uninit(void) {
		
		for (CCollision* col : _Collisionlist) {
			delete col;
		}
		_Collisionlist.clear();
		for (int i = 0; i < 5; i++) {
			for (CGameObject* object : _Objectlist[i]) {
				object->Uninit();
				delete object;
			}
			_Objectlist[i].clear();

		}
		
	}
	virtual void Update(void) {
		int count = 0;
		int size = _Collisionlist.size();
		if (size > 1) {
			std::list<CCollision*>::iterator itr;
			for (CCollision* col : _Collisionlist) {
				itr = _Collisionlist.begin();
				itr++;
				for (int j = 0; j < count; j++) {
					itr++;
				}
				for (int i = count; i < (size - 1); itr++, i++) {
					col->Dispatch(*itr);
				}
				count++;
			}
			_Collisionlist.remove_if([](CCollision* collision) {
				return collision->Destroy();
			});
		}

		

		for (int i = 0; i < 5; i++) {
			for (CGameObject* object : _Objectlist[i]) {
				object->Update();
			}
			
			_Objectlist[i].remove_if([](CGameObject* gameobject) {
				return gameobject->Destroy();
			});
		}
		
	}
	virtual void Draw(void) {
		for (int i = 0; i < 5; i++) {
			for (CGameObject* object : _Objectlist[i]) {
				object->Draw();
				
			}

		}
	}


};

/*
	for���̐V����������(C++ 11�ȍ~)
	(CGameObject* object : _Objectlist)
	��':'�̉E���́A�R���e�i�ł��邱��(list,vector�Ȃ�)

	������
	�E���̃R���e�i�̐擪����A�����̓���q�ɒl�����Ă����
	���I�[�iNULL�j�ɂȂ����烋�[�v�E�o

	�e���v���[�g�́A���܂藐�p���Ȃ����ƁI
	cpp�ɏ����ƃG���[���ł�

*/

/*
	���C���[
	�J�����F�O
	�w�i�F�P
	�RD�F�Q
	�G�t�F�N�g�F�R
	�QD�F�S
*/
