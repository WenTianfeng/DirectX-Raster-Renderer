#include"MeshRenderer.h"


MeshRenderer::MeshRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string modelFilePath):
	m_dxDevice(device),
	m_dxDeviceContext(deviceContext)
{
	this->m_componentName = "Mesh Renderer";

	if (!this->LoadModel(modelFilePath))
	{
		ErrorLogger::Log("Failed to load model.");
	}
}


void MeshRenderer::Render()
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


			//===============缓冲设置================
			//设置顶点缓冲
			UINT offsets = 0;
			this->m_dxDeviceContext->IASetVertexBuffers(0, 1, m_meshes[i].GetVertexBuffer().GetAddressOf(), m_meshes[i].GetVertexBuffer().StridePtr(), &offsets);

			//设置索引缓冲
			this->m_dxDeviceContext->IASetIndexBuffer(m_meshes[i].GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);


			//=============着色器资源更新==============

			for (std::map<std::string, ShaderParameter*>::value_type value : materialManager->materials[i].shaderParametersMap)
			{
				value.second->Bind(m_dxDeviceContext);

				if (value.second->GetName() == "CB_VS_TransformMatrix")
				{
					CB_VS_TransformMatrix transformMatrix = {};

					//更新世界矩阵
					DirectX::XMMATRIX W = owner->GetComponent<Transform>()->GetLocalToWorldMatrixXM();
					transformMatrix.world = DirectX::XMMatrixTranspose(W);

					//对世界矩阵求逆转置矩阵
					DirectX::XMMATRIX A = W;
					A.r[3] = DirectX::g_XMIdentityR3;
					transformMatrix.worldInverseTranspose = XMMatrixTranspose(XMMatrixTranspose(XMMatrixInverse(nullptr, A)));

					//更新视矩阵
					DirectX::XMMATRIX V = SceneManager::mainCamera->GetComponent<Camera>()->GetViewMatrix();
					transformMatrix.view = DirectX::XMMatrixTranspose(V);

					//更新投影矩阵
					DirectX::XMMATRIX P = SceneManager::mainCamera->GetComponent<Camera>()->GetProjectionMatrix();
					transformMatrix.projection = DirectX::XMMatrixTranspose(P);

					value.second->constantBuffer->SetStructure(&transformMatrix, sizeof(CB_VS_TransformMatrix));

				}

				//更新 ShaderParameter 包含的特定类型的资源
				value.second->UpdateParameterResource(this->m_dxDeviceContext);
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


bool MeshRenderer::LoadModel(std::string modelFilePath)
{
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile(modelFilePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!scene) 
	{
		return false;
	}

	this->ProcessNode(scene->mRootNode, scene);

	return true;
}


bool MeshRenderer::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->m_meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}

	return true;
}


Mesh MeshRenderer::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex3D> vertices;
	std::vector<DWORD> indices;


	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex3D vertex = {};

		//获取位置
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		//获取法线
		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);

	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(this->m_dxDevice,vertices,indices);
}

