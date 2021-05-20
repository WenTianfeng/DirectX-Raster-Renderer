#include "Object.h"

Object::Object(SceneManager* ownerManager, int UID, bool active):
	m_ownerManager(ownerManager),
	m_UID(UID),
	m_active(active)
{
	
}

Object::~Object()
{
}

bool Object::IsActive() const
{
	return this->m_active;
}

void Object::SetActivation(bool active)
{
	this->m_active = active;
}

int Object::GetUID() const
{
	return this->m_UID;
}

SceneManager* Object::GetOwnerManager()
{
	return this->m_ownerManager;
}


bool Object::Initialize()
{
	return true;
}

void Object::Update(float dt)
{
	if (m_active)
	{
		for (auto& component : this->m_components)
		{
			component->Update(dt);
		}
	}
}

void Object::Render()
{
	if (m_active)
	{
		for (auto& component : this->m_components)
		{
			component->Render();
		}
	}
}

void Object::Destroy()
{
	for (auto& component : this->m_components)
	{
		component->Destroy();
	}

}