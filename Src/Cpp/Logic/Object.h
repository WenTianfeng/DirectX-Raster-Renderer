#pragma once

#include<vector>
#include<map>
#include<typeinfo>


#include"..\Components\Component.h"

class SceneManager;

class Object 
{

private:
    int m_UID;
    bool m_active;
    SceneManager* m_ownerManager;

public:
    bool IsActive() const;
    void SetActivation(bool active);
    int GetUID() const;
    SceneManager* GetOwnerManager();

public:
	Object(SceneManager* ownerManager, int UID,bool active = true);
	~Object();

	bool Initialize();
	void Update(float dt);
    void Render();
	void Destroy();

    //ģ�庯�������ָ�����͵�component
    template <typename T, typename...TArgs>
    bool AddComponent(TArgs&&...args) 
    {
        //��������Ѿ�ӵ�и�����������򷵻�
        if (this->HasComponent<T>())
        {
            return false;
        }

        //ʵ�������
        T* newComponent(new T(std::forward<TArgs>(args)...));

        //�趨�����owner
        newComponent->owner = this;

        //�����ʼ��
        newComponent->Initialize();

        //��������뵽����б�
        m_components.emplace_back(newComponent);

        //��������뵽���map
        m_componentsMap[&typeid(*newComponent)] = newComponent;
        
        return true;
    }

    //ģ�庯��������ָ�����͵����
    template <typename T>
    T* GetComponent() 
    {
        return static_cast<T*>(m_componentsMap[&typeid(T)]);
    }

    //ģ�庯�����ж������Ƿ�ӵ�и��������
    template <typename T>
    bool HasComponent()
    {
        return m_componentsMap.count(&typeid(T)) == 1;
    }

private:
	std::vector<Component*> m_components;//����б�

	std::map<const std::type_info*, Component*> m_componentsMap;//���map��������ͣ��������ָ�룩
};
