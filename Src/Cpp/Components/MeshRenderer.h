#pragma once

//===assimp��̬���ӿ�===
#pragma comment(lib,"assimp-vc142-mtd.lib")

//===c++��׼��===
#include<vector>
#include<string>

//===assimp������===
#include<Assimp/Importer.hpp>
#include<Assimp/postprocess.h>
#include<Assimp/scene.h>


#include"..\Graphics\Graphics.h"
#include"..\Graphics\Structures\Mesh.h"
#include"Transform.h"
#include"Camera.h"
#include"MaterialManager.h"
#include"Light.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::string modelFilePath);
	void Render();

protected:
	/// <summary>
	/// ʹ��assimp����ģ��
	/// </summary>
	/// <param name="modelFilePath">ģ���ļ�·��</param>
	/// <returns>�Ƿ���سɹ�</returns>
	bool LoadModel(std::string modelFilePath);

	/// <summary>
	/// ����assimp�ڵ�
	/// </summary>
	/// <param name="node">�ڵ�ָ��</param>
	/// <param name="scene">����ָ��</param>
	/// <returns></returns>
	bool ProcessNode(aiNode* node, const aiScene* scene);

	/// <summary>
	/// ����assimp����
	/// </summary>
	/// <param name="mesh">����ָ��</param>
	/// <param name="scene">����ָ��</param>
	/// <returns>�Զ���Mesh�ṹ����</returns>
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

protected:
	//Mesh �б�
	std::vector<Mesh> m_meshes;

	//DirectX �豸
	ID3D11Device* m_dxDevice = nullptr;
	//DirectX ������
	ID3D11DeviceContext* m_dxDeviceContext = nullptr;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexSRV;
};


