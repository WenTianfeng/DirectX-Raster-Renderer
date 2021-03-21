#pragma once

#include<vector>

#include"..\Logic\Component.h"

#include"Structures\Material.h"

class MaterialManager : public Component
{
public:
	MaterialManager();

	void Initialize();
	void Update();

private:
	std::vector<Material> m_materials;
};