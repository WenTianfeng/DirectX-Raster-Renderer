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

#include"..\Logic\SceneManager.h"
#include"..\Graphics\Structures\Mesh.h"
#include"Transform.h"
#include"Camera.h"
#include"MaterialManager.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string meshFilePath);
	
	void Initialize();
	void Render();
	
	UINT GetMeshCount();


protected:
	/// <summary>
	/// 使用assimp加载模型
	/// </summary>
	/// <param name="modelFilePath">模型文件路径</param>
	/// <returns>是否加载成功</returns>
	bool LoadModel(std::string modelFilePath);

	/// <summary>
	/// 处理assimp节点
	/// </summary>
	/// <param name="node">节点指针</param>
	/// <param name="scene">场景指针</param>
	/// <returns></returns>
	bool ProcessNode(aiNode* node, const aiScene* scene);

	/// <summary>
	/// 处理assimp网格
	/// </summary>
	/// <param name="mesh">网格指针</param>
	/// <param name="scene">场景指针</param>
	/// <returns>自定义Mesh结构对象</returns>
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);


protected:
	std::string m_meshFilePath;
	std::vector<Mesh> m_meshes;//Mesh 列表
	ID3D11Device* m_dxDevice = nullptr;//DirectX 设备
	ID3D11DeviceContext* m_dxDeviceContext = nullptr;//DirectX 上下文
};


