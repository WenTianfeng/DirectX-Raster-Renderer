#pragma once

#include<vector>
#include<DirectXMath.h>
#include <d3d11_1.h>//DirectX 11.1������ã�https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/��

#include"..\Tools\ErrorLogger.h"
#include"..\Object\Object.h"


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

private:
	/// <summary>
	/// �������ݳ�ʼ��
	/// </summary>
	/// <returns>��������Ƿ��ʼ���ɹ�</returns>
	bool InitializeScene(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

public:
	static Object* mainCamera;//���������
	static std::vector<Object*> lights;//������Դ�б�
	static std::vector<Object*> objects;//���������б�

};