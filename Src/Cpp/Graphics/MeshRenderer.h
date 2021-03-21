#pragma once

//===assimp静态链接库===
#pragma comment(lib,"assimp-vc142-mtd.lib")

//===c++标准库===
#include<vector>
#include<string>

//===assimp库引用===
#include<Assimp/Importer.hpp>
#include<Assimp/postprocess.h>
#include<Assimp/scene.h>


#include"..\Logic\Component.h"

#include"Structures\Mesh.h"
#include"MaterialManager.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer(ID3D11Device* device, std::string modelFilePath);
	void Initialize();
	void Update(float dt);
	void Render();

private:
	bool LoadModel(std::string modelFilePath);
	bool ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:

	ID3D11Device* m_device = nullptr;

	std::vector<Mesh> m_meshes;

};


