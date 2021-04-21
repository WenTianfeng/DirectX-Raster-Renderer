#include "SceneManager.h"
#include"..\Components\Skybox.h"
#include"..\Logic\ObjectScripts\CameraControl.h"

//ȫ�־�̬������ʼ��
Object* SceneManager::mainCamera = nullptr;
std::vector<Object*> SceneManager::lights = {};

bool SceneManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//��ʼ������
	if (!InitializeScene(device,deviceContext))
	{
		return false;
	}

	return true;
}

void SceneManager::Update(float dt)
{
	//�����г���������и���
	for (auto object : this->m_objects)
	{
		object->Update(dt);
	}
}

std::vector<Object*> SceneManager::GetObjects()
{
	return this->m_objects;
}

bool SceneManager::InitializeScene(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
#pragma region �ж�shader�ļ�·��

	std::wstring shaderFolderPath;
	if (IsDebuggerPresent())
	{
#ifdef _DEBUG
		shaderFolderPath = L"Debug\\";
#else //RELEASE�����޸ģ�
		shaderFolderPath = L"..\\..\\..\\Debug\\";
#endif // DEBUG

	}
#pragma endregion

#pragma region �������

	Object* camera = new Object();
	camera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	camera->AddComponent<Camera>(DirectX::XM_PI / 3, 2.0f, 0.5f, 2000.0f);
	camera->AddComponent<CameraControl>();
	mainCamera = camera;

	this->m_objects.push_back(camera);//����������б���

#pragma endregion

#pragma region ���ɹ�Դ

	Object* directional_light1 = new Object();
	directional_light1->AddComponent<Transform>(DirectX::XMFLOAT3(80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directional_light1->AddComponent<Light>(Light::LightType::Directional, DirectX::XMFLOAT4(1.0f, 0, 0, 0), 1.0f, 2000.0f);
	this->m_objects.push_back(directional_light1);
	lights.push_back(directional_light1);

	Object* directional_light2 = new Object();
	directional_light2->AddComponent<Transform>(DirectX::XMFLOAT3(-80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, -DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directional_light2->AddComponent<Light>(Light::LightType::Directional, DirectX::XMFLOAT4(0, 0, 1.0f, 0), 1.0f, 2000.0f);
	this->m_objects.push_back(directional_light2);
	lights.push_back(directional_light2);

#pragma endregion

#pragma region ������պ�

	Object* skybox = new Object();
	skybox->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(500, 500, 500));

	std::vector<std::wstring> skyboxTextureList = {
		L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Left.png", L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Right.png",
			L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Up.png", L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Down.png",
			L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Front.png", L"Resources\\Textures\\SkyboxTexturesCloudy\\CloudyCrown_Sunset_Back.png" };

	skybox->AddComponent<Skybox>(device, deviceContext, skyboxTextureList);

	std::wstring skyboxVertexShaderFilePaths[] = {
		shaderFolderPath + L"VertexShader_Skybox.cso"
	};

	std::wstring skyboxPixelShaderFilePaths[] = {
		shaderFolderPath + L"PixelShader_Skybox.cso"
	};

	skybox->AddComponent<MaterialManager>(device, skyboxVertexShaderFilePaths, skyboxPixelShaderFilePaths, 1);

	//this->m_objects.push_back(skybox);

#pragma endregion

#pragma region ���ɳ�������

	//����������
	Object* cube = new Object();
	cube->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(DirectX::XM_PI / 2, DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));//���Transform���
	cube->AddComponent<MeshRenderer>(device, deviceContext, "Resources\\Models\\BoxBall.FBX");//���MeshRender���
	
	std::wstring vertexShaderFilePaths[] = {
		shaderFolderPath + L"VertexShader.cso",
		shaderFolderPath + L"VertexShader.cso"
	};

	std::wstring pixelShaderFilePaths[] = {
		shaderFolderPath + L"PixelShader_PBR.cso",
		shaderFolderPath + L"PixelShader_PBR.cso"
	};

	cube->AddComponent<MaterialManager>(device, vertexShaderFilePaths, pixelShaderFilePaths, 2);//���MaterialManager���

	this->m_objects.push_back(cube);

	//�����ı���
	Object* quad = new Object();
	quad->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(3, 3, 3));//���Transform���
	quad->AddComponent<MeshRenderer>(device, deviceContext, "Resources\\Models\\Quad.FBX");//���MeshRender���

	std::wstring quadVertexShaderFilePaths[] = {
		shaderFolderPath + L"VertexShader.cso",
	};

	std::wstring quadPixelShaderFilePaths[] = {
		shaderFolderPath + L"PixelShader_Texture.cso",
	};

	quad->AddComponent<MaterialManager>(device, quadVertexShaderFilePaths, quadPixelShaderFilePaths, 1);//���MaterialManager���

	//this->m_objects.push_back(quad);

#pragma endregion

	return true;
}
