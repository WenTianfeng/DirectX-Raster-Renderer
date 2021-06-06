#pragma once

#include"MeshRenderer.h"
#include<DirectXTK\WICTextureLoader.h>

class Skybox : public MeshRenderer
{
public:
	Skybox(ID3D11Device* device, ID3D11DeviceContext* deviceContext,const std::string skyboxShaderFilePath, const std::vector<std::wstring>& cubeMapFileNames);
	void Initialize();
	void Render();

private:
	std::string m_skyboxShaderFilePath;
	Material m_skyboxMaterial;
	std::vector<std::wstring> m_cubeMapFileNames;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_skyboxTextureCubeSRV;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_skyboxTextureCubeSampler;


};