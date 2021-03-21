#include"MeshRenderer.h"


MeshRenderer::MeshRenderer(ID3D11Device* device, std::string modelFilePath)
{
	this->LoadModel(modelFilePath);
}

void MeshRenderer::Initialize()
{

}

void MeshRenderer::Update(float dt)
{
	
}


void MeshRenderer::Render()
{
	for (UINT i = 0; i < this->m_meshes.size(); i++)
	{

		//设置顶点着色器


		//设置像素着色器


		//=======状态设置========

			//设置输入布局

			//设置图元类型

			//设置深度-模板缓冲状态

			//设置混合状态

			//设置采样器状态

		//=======缓冲设置========

			//设置顶点缓冲


			//设置索引缓冲


			//设置常量缓冲

		//=======绘制========
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

	return Mesh(this->m_device,vertices,indices);
}
