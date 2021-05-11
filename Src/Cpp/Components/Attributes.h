#pragma once

#include"Component.h"

class Attributes:public Component
{

private:
	std::string m_objectName;
	std::string m_tag;
	bool m_displayInHierarchy;

public:
	Attributes(std::string objectName, std::string tag = "Untagged", bool displayInHierarchy = true);
	void UpdateProperties(std::string objectName, std::string tag);

public:
	std::string ObjectName() const;
	std::string ObjectTag() const;
	bool IsDisplayInHierarchy() const;

	void SetObjectName(std::string objectName);
	void SetObjectTag(std::string tag);
	void SetDisplayInHierarchy(bool displayInHierarchy);
};