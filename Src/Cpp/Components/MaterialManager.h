#pragma once

#include<vector>

#include"Component.h"

#include"..\Graphics\Structures\Material.h"
#include"MeshRenderer.h"
#include"..\Tools\FileManager.h"

class MaterialManager : public Component
{
public:
	MaterialManager(ID3D11Device* device);

	void Initialize();
public:
	std::vector<Material> materials;

private:
	//DirectX …Ë±∏
	ID3D11Device* m_dxDevice = nullptr;
	
};