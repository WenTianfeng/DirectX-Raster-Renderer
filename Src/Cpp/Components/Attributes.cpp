#include "Attributes.h"

Attributes::Attributes(std::string objectName, std::string tag):
	m_objectName(objectName),
	m_tag(tag)
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
