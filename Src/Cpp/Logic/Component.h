#pragma once



class Object;

class Component 
{
public:
    Object* owner = nullptr;
    virtual ~Component() {}
    virtual void Initialize() {}
    virtual void Update(float dt) {}
    virtual void Render() {}

};