#pragma once

#include<vector>

#include"Component.h"

#include"..\Graphics\Structures\Material.h"

class MaterialManager : public Component
{
public:
	MaterialManager(ID3D11Device* device, std::wstring* vertexShaderFiles, std::wstring* pixelShaderFiles, UINT shaderCount);

public:
	std::vector<Material> materials;

private:
	//DirectX …Ë±∏
	ID3D11Device* m_dxDevice = nullptr;
};