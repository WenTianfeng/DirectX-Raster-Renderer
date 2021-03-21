#pragma once

#include<vector>
#include<map>
#include<typeinfo>


#include"Component.h"


class Object 
{
public:
	Object();
	~Object();

	bool Initialize();
	void Update(float dt);
    void Render();
	void Destroy();

    //ģ�庯�������ָ�����͵�component
    template <typename T, typename...TArgs>
    T& AddComponent(TArgs&&...args) {
        T* newComponent(new T(std::forward<TArgs>(args)...));
        newComponent->owner = this;
        m_components.emplace_back(newComponent);
        m_componentsMap[&typeid(*newComponent)] = newComponent;
        newComponent->Initialize();
        return *newComponent;
    }

    //ģ�庯��������ָ�����͵����
    template <typename T>
    T* GetComponent() {
        return static_cast<T*>(m_componentsMap[&typeid(T)]);
    }


private:
	std::vector<Component*> m_components;
	std::map<const std::type_info*, Component*> m_componentsMap;
};
