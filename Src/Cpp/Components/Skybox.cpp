#include "Skybox.h"

Skybox::Skybox(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::vector<std::wstring>& cubeMapFileNames):
	MeshRenderer(device,deviceContext, "Resources\\Models\\SkyboxSphere.FBX"),
	m_cubeMapFileNames(cubeMapFileNames)
{
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

	ID3D11Texture2D* texArray = nullptr;
	this->m_dxDevice->CreateTexture2D(&texArrayDesc, nullptr, &texArray);

	texArray->GetDesc(&texArrayDesc);

	for (UINT i = 0; i < 6; ++i)
	{
		for (UINT j = 0; j < texArrayDesc.MipLevels; ++j)
		{
			this->m_dxDeviceContext->CopySubresourceRegion(texArray, D3D11CalcSubresource(j, i, texArrayDesc.MipLevels), 0, 0, 0, srcTexVec[i], j, nullptr);
		}
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = texArrayDesc.MipLevels;

	this->m_dxDevice->CreateShaderResourceView(texArray, &viewDesc, this->m_skyboxTextureCubeSRV.GetAddressOf());


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

	MaterialManager* materialManager = this->owner->GetComponent<MaterialManager>();
	if (!materialManager)
	{
		ErrorLogger::Log("There is no MaterialManager Component on this object.");
		exit(-1);
	}

	for (UINT i = 0; i < this->m_meshes.size(); i++)
	{

		//设置顶点着色器
		this->m_dxDeviceContext->VSSetShader(materialManager->materials[i].GetVertexShader().GetShader(), 0, 0);

		//设置像素着色器
		this->m_dxDeviceContext->PSSetShader(materialManager->materials[i].GetPixelShader().GetShader(), 0, 0);

		//=======状态设置========

		//设置输入布局
		this->m_dxDeviceContext->IASetInputLayout(materialManager->materials[i].GetInputLayout());

		//设置图元类型
		this->m_dxDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//设置深度-模板缓冲状态
		this->m_dxDeviceContext->OMSetDepthStencilState(materialManager->materials[i].GetDepthStencilState(), 0);

		//设置混合状态
		this->m_dxDeviceContext->OMSetBlendState(materialManager->materials[i].GetBlendState(), nullptr, 0xffffffff);

		//设置采样器状态
		this->m_dxDeviceContext->PSSetSamplers(0, 1, this->m_skyboxTextureCubeSampler.GetAddressOf());

		//设置纹理资源
		this->m_dxDeviceContext->PSSetShaderResources(1, 1, this->m_skyboxTextureCubeSRV.GetAddressOf());


		//=======缓冲设置========

		//设置顶点缓冲
		UINT offsets = 0;
		this->m_dxDeviceContext->IASetVertexBuffers(0, 1, m_meshes[i].GetVertexBuffer().GetAddressOf(), m_meshes[i].GetVertexBuffer().StridePtr(), &offsets);


		//设置索引缓冲
		this->m_dxDeviceContext->IASetIndexBuffer(m_meshes[i].GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);


		//设置常量缓冲
		this->m_dxDeviceContext->VSSetConstantBuffers(0, 1, materialManager->materials[i].GetConstantBuffer_TransformMatrix().GetAddressOf());

		//更新世界矩阵
		DirectX::XMMATRIX W = owner->GetComponent<Transform>()->GetLocalToWorldMatrixXM();
		materialManager->materials[i].GetConstantBuffer_TransformMatrix().bufferData.world = DirectX::XMMatrixTranspose(W);

		//对世界矩阵求逆转置矩阵
		DirectX::XMMATRIX A = W;
		A.r[3] = DirectX::g_XMIdentityR3;
		materialManager->materials[i].GetConstantBuffer_TransformMatrix().bufferData.worldInverseTranspose = XMMatrixTranspose(XMMatrixTranspose(XMMatrixInverse(nullptr, A)));

		//更新视矩阵
		DirectX::XMMATRIX V = Graphics::mainCamera->GetComponent<Camera>()->GetViewMatrix();
		materialManager->materials[i].GetConstantBuffer_TransformMatrix().bufferData.view = DirectX::XMMatrixTranspose(V);

		//更新投影矩阵
		DirectX::XMMATRIX P = Graphics::mainCamera->GetComponent<Camera>()->GetProjectionMatrix();
		materialManager->materials[i].GetConstantBuffer_TransformMatrix().bufferData.projection = DirectX::XMMatrixTranspose(P);

		materialManager->materials[i].GetConstantBuffer_TransformMatrix().UpdateConstantBuffer(this->m_dxDeviceContext);

		//设置结构缓冲
		this->m_dxDeviceContext->PSSetShaderResources(0, 1, materialManager->materials[i].GetStructuredBuffer_Light().GetSRVAddressOf());

		//=======绘制========
		this->m_dxDeviceContext->DrawIndexed(this->m_meshes[i].GetIndexBuffer().IndexCount(), 0, 0);
	}
}


