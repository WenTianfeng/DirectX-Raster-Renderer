#pragma once

#include<string>

class Object;

class Component 
{

protected:
    std::string m_componentName;

public:
    Object* owner = nullptr;
    virtual ~Component() {}
    virtual void Initialize() {}
    virtual void Update(float dt) {}
    virtual void Render() {}
    virtual void Destroy() {}

    std::string GetComponentName() 
    {
        return this->m_componentName;
    }
};