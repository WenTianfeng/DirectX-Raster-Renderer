#include "Skybox.h"

Skybox::Skybox(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<std::wstring>& cubeMapFileNames):
	MeshRenderer(device,deviceContext, "Assets\\Models\\SkyboxSphere.FBX"),
	m_cubeMapFileNames(cubeMapFileNames)
{
	this->m_componentName = "Skybox";
}

void Skybox::Initialize()
{
	std::vector<ID3D11Texture2D*> srcTexVec(6, nullptr);
	std::vector<D3D11_TEXTURE2D_DESC> texDescVec(6);

	for (UINT i = 0; i < 6; ++i)
	{
		HRESULT hr = DirectX::CreateWICTextureFromFile(this->m_dxDevice, nullptr, this->m_cubeMapFileNames[i].c_str(), (ID3D11Resource**)&srcTexVec[i], nullptr);
		COM_ERROR_IF_FAILED(hr, "Failed to create DirectX WICTexture From File.");
		srcTexVec[i]->GetDesc(&texDescVec[i]);
	}

	D3D11_TEXTURE2D_DESC texArrayDesc = {};
	texArrayDesc.Width = texDescVec[0].Width;
	texArrayDesc.Height = texDescVec[0].Height;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = 6;
	texArrayDesc.Format = texDescVec[0].Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texArray;
	this->m_dxDevice->CreateTexture2D(&texArrayDesc, nullptr, texArray.GetAddressOf());

	texArray->GetDesc(&texArrayDesc);

	for (UINT i = 0; i < 6; ++i)
	{
		for (UINT j = 0; j < texArrayDesc.MipLevels; ++j)
		{
			this->m_dxDeviceContext->CopySubresourceRegion(texArray.Get(), D3D11CalcSubresource(j, i, texArrayDesc.MipLevels), 0, 0, 0, srcTexVec[i], j, nullptr);
		}
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = texArrayDesc.MipLevels;

	this->m_dxDevice->CreateShaderResourceView(texArray.Get(), &viewDesc, this->m_skyboxTextureCubeSRV.GetAddressOf());


	// 初始化采样器状态
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = this->m_dxDevice->CreateSamplerState(&sampDesc, this->m_skyboxTextureCubeSampler.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create DirectX SamplerState.");

}

void Skybox::Render()
{
	//如果Object拥有MaterialManager组件
	if (this->owner->HasComponent<MaterialManager>())
	{
		//获取 MaterialManager 组件
		MaterialManager* materialManager = this->owner->GetComponent<MaterialManager>();

		//遍历拥有的所有 Mesh
		for (UINT i = 0; i < this->m_meshes.size(); i++)
		{

			//================着色器设置================
			materialManager->materials[i].BindShaders(m_dxDeviceContext);

			//===============状态设置================
			//设置输入布局
			this->m_dxDeviceContext->IASetInputLayout(materialManager->materials[i].GetInputLayout());

			//设置图元类型
			this->m_dxDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//设置深度-模板缓冲状态
			this->m_dxDeviceContext->OMSetDepthStencilState(materialManager->materials[i].GetDepthStencilState(), 0);

			//设置混合状态
			this->m_dxDeviceContext->OMSetBlendState(materialManager->materials[i].GetBlendState(), nullptr, 0xffffffff);


			this->m_dxDeviceContext->PSSetSamplers(0, 1, m_skyboxTextureCubeSampler.GetAddressOf());
			this->m_dxDeviceContext->PSSetShaderResources(1, 1, m_skyboxTextureCubeSRV.GetAddressOf());

			//===============缓冲设置================
			//设置顶点缓冲
			UINT offsets = 0;
			this->m_dxDeviceContext->IASetVertexBuffers(0, 1, m_meshes[i].GetVertexBuffer().GetAddressOf(), m_meshes[i].GetVertexBuffer().StridePtr(), &offsets);

			//设置索引缓冲
			this->m_dxDeviceContext->IASetIndexBuffer(m_meshes[i].GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);


			//=============着色器资源更新==============

			for (std::map<std::string, ShaderParameter*>::value_type pair_name_shaderParameter : materialManager->materials[i].shaderParametersMap)
			{
				pair_name_shaderParameter.second->Bind(m_dxDeviceContext);

				if (pair_name_shaderParameter.first == "CB_PresetVariables")
				{
					CB_PresetVariables presetVariables = {};

					//更新世界矩阵
					DirectX::XMMATRIX W = owner->GetComponent<Transform>()->GetLocalToWorldMatrixXM();
					presetVariables.world = DirectX::XMMatrixTranspose(W);

					//对世界矩阵求逆转置矩阵
					DirectX::XMMATRIX A = W;
					A.r[3] = DirectX::g_XMIdentityR3;
					presetVariables.worldInverseTranspose = XMMatrixTranspose(XMMatrixTranspose(XMMatrixInverse(nullptr, A)));

					//更新视矩阵
					DirectX::XMMATRIX V = owner->GetOwnerManager()->GetMainCamera()->GetComponent<Camera>()->GetViewMatrix();
					presetVariables.view = DirectX::XMMatrixTranspose(V);

					//更新投影矩阵
					DirectX::XMMATRIX P = owner->GetOwnerManager()->GetMainCamera()->GetComponent<Camera>()->GetProjectionMatrix();
					presetVariables.projection = DirectX::XMMatrixTranspose(P);

					presetVariables.viewPos = owner->GetOwnerManager()->GetMainCamera()->GetComponent<Camera>()->GetViewPos();
					presetVariables.padding = 0;

					pair_name_shaderParameter.second->constantBuffer->SetStructure(&presetVariables, sizeof(CB_PresetVariables));

				}

				//更新 ShaderParameter 包含的特定类型的资源
				pair_name_shaderParameter.second->UpdateParameterResource(this->m_dxDeviceContext);
			}

			//===============绘制================
			this->m_dxDeviceContext->DrawIndexed(this->m_meshes[i].GetIndexBuffer().IndexCount(), 0, 0);

		}
	}
	else    //如果Object没有MaterialManager组件
	{
		ErrorLogger::Log("There is no MaterialManager Component on this object.");
		exit(-1);

	}

}




