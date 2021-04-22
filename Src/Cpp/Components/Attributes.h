#pragma once

#include"Component.h"

class Attributes:public Component
{

private:
	std::string m_objectName;
	std::string m_tag;

public:
	Attributes(std::string objectName, std::string tag = "Untagged");
	void UpdateProperties(std::string objectName, std::string tag);

public:
	std::string ObjectName() const;
	std::string ObjectTag() const;

	void SetObjectName(std::string objectName);
	void SetObjectTag(std::string tag);
};