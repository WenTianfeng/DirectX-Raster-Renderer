#pragma once

#include<vector>
#include<map>
#include<typeinfo>

#include"..\Components\Component.h"

class SceneManager;

class Object 
{

private:
    unsigned int m_UID;
    bool m_active;
    SceneManager* m_ownerManager;

public:
    bool IsActive() const;
    void SetActivation(bool active);
    unsigned int GetUID() const;
    SceneManager* GetOwnerManager();

public:
	Object(SceneManager* ownerManager, unsigned int UID,bool active = true);
	~Object();

	void Update(float dt);
    void Render();
	void Destroy();

public:
    //模板函数：添加指定类型的component
    template <typename T, typename...TArgs>
    bool AddComponent(TArgs&&...args) 
    {
        //如果对象已经拥有该类型组件，则返回
        if (this->HasComponent<T>())
        {
            return false;
        }

        //实例化组件
        T* newComponent(new T(std::forward<TArgs>(args)...));

        //设定组件的owner
        newComponent->owner = this;

        //组件初始化
        newComponent->Initialize();

        //将组件加入到组件列表
        m_components.emplace_back(newComponent);

        //将组件加入到组件map
        m_componentsMap[&typeid(*newComponent)] = newComponent;
        
        return true;
    }

    //模板函数：删除指定类型的组件
    template <typename T>
    bool RemoveComponent()
    {
        //如果对象没有拥有该类型组件，则返回
        if (!this->HasComponent<T>())
        {
            return false;
        }
        unsigned int targetIndex = 0;
        //遍历组件列表
        for (unsigned int i = 0; i < m_components.size(); i++)
        {
            //如果当前组件的类型名和参数类型名相同，则记录其位置
            if (typeid(T) == typeid(*m_components[i]))
            {
                targetIndex = i;
                break;
            }
        }
        //从列表中删除组件
        m_components.erase(m_components.begin()+ targetIndex);
        //从哈希表中删除组件
        m_componentsMap.erase(&typeid(T));

        return true;
    }

    //模板函数：返回指定类型的组件
    template <typename T>
    T* GetComponent() 
    {
        if (!HasComponent<T>())
        {
            return nullptr;
        }

        return static_cast<T*>(m_componentsMap[&typeid(T)]);
    }

    //模板函数：判定对象是否拥有该类型组件
    template <typename T>
    bool HasComponent()
    {
        return m_componentsMap.count(&typeid(T)) == 1;
    }

private:
	std::vector<Component*> m_components;//组件列表
	std::map<const std::type_info*, Component*> m_componentsMap;//组件map（组件类型：组件对象指针）
};
