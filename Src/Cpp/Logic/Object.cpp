#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

bool Object::Initialize()
{
	return false;
}

void Object::Update(float dt)
{
	for (auto& component : this->m_components)
	{
		component->Update(dt);
	}
}

void Object::Render()
{
	for (auto& component : this->m_components)
	{
		component->Render();
	}
}

void Object::Destroy()
{


}
