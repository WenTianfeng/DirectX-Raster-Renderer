#include "Attributes.h"

Attributes::Attributes(std::string objectName, std::string tag, bool displayInHierarchy):
	m_objectName(objectName),
	m_tag(tag),
	m_displayInHierarchy(displayInHierarchy)
{
	this->m_componentName = "Attributes";
}

void Attributes::UpdateProperties(std::string objectName, std::string tag)
{
	SetObjectName(objectName);
	SetObjectTag(tag);
}

std::string Attributes::ObjectName() const
{
	return this->m_objectName;
}

std::string Attributes::ObjectTag() const
{
	return this->m_tag;
}

void Attributes::SetObjectName(std::string objectName)
{
	this->m_objectName = objectName;
}

void Attributes::SetObjectTag(std::string tag)
{
	this->m_tag = tag;
}

void Attributes::SetDisplayInHierarchy(bool displayInHierarchy)
{
	this->m_displayInHierarchy = displayInHierarchy;
}

bool Attributes::IsDisplayInHierarchy() const
{
	return m_displayInHierarchy;
}
