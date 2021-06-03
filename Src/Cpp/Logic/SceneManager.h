#pragma once

#include<vector>
#include<DirectXMath.h>
#include <d3d11_1.h>//DirectX 11.1������ã�https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/��

#include"..\Logic\Object.h"
#include"..\Tools\ErrorLogger.h"


class SceneManager
{

public:

	SceneManager();
	~SceneManager();

	/// <summary>
	/// ������������ʼ��
	/// </summary>
	/// <param name="device">DirectX�豸</param>
	/// <param name="deviceContext">DirectX������</param>
	/// <returns>�Ƿ��ʼ���ɹ�</returns>
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	/// <summary>
	/// ÿ֡�������
	/// </summary>
	/// <param name="dt">��֡ʱ��</param>
	void Update(float dt);

	/// <summary>
	/// �������ź���ã������´��ڴ�С�޸��������
	/// </summary>
	/// <param name="clientWidth">�´��ڿ��</param>
	/// <param name="clientHeight">�´��ڸ߶�</param>
	void OnWindowResize(int clientWidth, int clientHeight);


	//Get����
	Object* GetMainCamera();
	std::vector<Object*> GetLights();
	std::vector<Object*> GetObjects();

	/// <summary>
	/// ���ʵ�����
	/// </summary>
	/// <param name="objectName"></param>
	/// <param name="objectMeshFilePath"></param>
	void AddObject(std::string objectName, std::string objectMeshFilePath = "");

	/// <summary>
	/// ɾ��ʵ�����
	/// </summary>
	/// <param name="objectID">����ID</param>
	/// <returns>�Ƿ�ɾ���ɹ�</returns>
	bool DeleteObject(UINT objectID);

	/// <summary>
	/// ɾ��ʵ�����
	/// </summary>
	/// <param name="object">����ָ��</param>
	/// <returns>�Ƿ�ɾ���ɹ�</returns>
	bool DeleteObject(Object* object);

private:
	/// <summary>
	/// �������ݳ�ʼ��
	/// </summary>
	/// <returns>��������Ƿ��ʼ���ɹ�</returns>
	bool InitializeScene();

	UINT NewObjectID();
	std::string ObtainNewObjectNameWithPostfix(std::string objectName, UINT objectID);

private:
	ID3D11Device* m_dxDevice;//Dx�豸
	ID3D11DeviceContext* m_dxDeviceContext;//Dx������
	Object* m_mainCamera;//���������
	std::vector<Object*> m_lights;//������Դ�б�
	std::vector<Object*> m_objects;//���������б�

private:
	unsigned int m_objectIDCounter;
};
