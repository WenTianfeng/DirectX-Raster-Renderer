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
	//���Objectӵ��MaterialManager���
	if (this->owner->HasComponent<MaterialManager>())
	{

		//��ȡ MaterialManager ���
		MaterialManager* materialManager = this->owner->GetComponent<MaterialManager>();

		//����ӵ�е����� Mesh
		for (UINT i = 0; i < this->m_meshes.size(); i++)
		{

			//================��ɫ������================
			materialManager->materials[i].BindShaders(m_dxDeviceContext);

			//===============״̬����================
			//�������벼��
			this->m_dxDeviceContext->IASetInputLayout(materialManager->materials[i].GetInputLayout());

			//����ͼԪ����
			this->m_dxDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//�������-ģ�建��״̬
			this->m_dxDeviceContext->OMSetDepthStencilState(materialManager->materials[i].GetDepthStencilState(), 0);

			//���û��״̬
			this->m_dxDeviceContext->OMSetBlendState(materialManager->materials[i].GetBlendState(), nullptr, 0xffffffff);


			//===============��������================
			//���ö��㻺��
			UINT offsets = 0;
			this->m_dxDeviceContext->IASetVertexBuffers(0, 1, m_meshes[i].GetVertexBuffer().GetAddressOf(), m_meshes[i].GetVertexBuffer().StridePtr(), &offsets);

			//������������
			this->m_dxDeviceContext->IASetIndexBuffer(m_meshes[i].GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);


			//=============��ɫ����Դ����==============

			for (std::map<std::string, ShaderParameter*>::value_type value : materialManager->materials[i].shaderParametersMap)
			{
				value.second->Bind(m_dxDeviceContext);

				if (value.second->GetName() == "CB_VS_TransformMatrix")
				{
					CB_VS_TransformMatrix transformMatrix = {};

					//�����������
					DirectX::XMMATRIX W = owner->GetComponent<Transform>()->GetLocalToWorldMatrixXM();
					transformMatrix.world = DirectX::XMMatrixTranspose(W);

					//�������������ת�þ���
					DirectX::XMMATRIX A = W;
					A.r[3] = DirectX::g_XMIdentityR3;
					transformMatrix.worldInverseTranspose = XMMatrixTranspose(XMMatrixTranspose(XMMatrixInverse(nullptr, A)));

					//�����Ӿ���
					DirectX::XMMATRIX V = SceneManager::mainCamera->GetComponent<Camera>()->GetViewMatrix();
					transformMatrix.view = DirectX::XMMatrixTranspose(V);

					//����ͶӰ����
					DirectX::XMMATRIX P = SceneManager::mainCamera->GetComponent<Camera>()->GetProjectionMatrix();
					transformMatrix.projection = DirectX::XMMatrixTranspose(P);

					value.second->constantBuffer->SetStructure(&transformMatrix, sizeof(CB_VS_TransformMatrix));

				}

				//���� ShaderParameter �������ض����͵���Դ
				value.second->UpdateParameterResource(this->m_dxDeviceContext);
			}
			
			//===============����================
			this->m_dxDeviceContext->DrawIndexed(this->m_meshes[i].GetIndexBuffer().IndexCount(), 0, 0);

		}
	}
	else    //���Objectû��MaterialManager���
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

		//��ȡλ��
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		//��ȡ����
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

