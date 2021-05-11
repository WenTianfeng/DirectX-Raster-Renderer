#include "SceneManager.h"
#include"..\Components\Skybox.h"
#include"..\Components\Light.h"
#include"..\Components\Attributes.h"
#include"..\Logic\ObjectScripts\CameraControl.h"

//ȫ�־�̬������ʼ��
Object* SceneManager::mainCamera = nullptr;
std::vector<Object*> SceneManager::lights = {};
std::vector<Object*> SceneManager::objects = {};

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

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
	for (auto object : this->objects)
	{
		object->Update(dt);
	}
}

void SceneManager::OnWindowResize(int clientWidth, int clientHeight)
{
	//����������ĺ��ݱ�
	mainCamera->GetComponent<Camera>()->SetAspect((float)clientWidth / (float)clientHeight);

}

bool SceneManager::InitializeScene(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//��ɫ���ļ�·��
	std::string shaderFolderPath = "Assets\\Shaders\\";

//=================��������=================

	#pragma region �������

	//�༭ģʽ���
	Object* editModeCamera = new Object(GetNewObjectID());
	editModeCamera->AddComponent<Attributes>("EditMode Camera", "Camera", false);
	editModeCamera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 80.0f, -120.0f), DirectX::XMFLOAT3(DirectX::XM_PI / 12, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	editModeCamera->AddComponent<Camera>();
	editModeCamera->AddComponent<CameraControl>();
	mainCamera = editModeCamera;
	this->objects.push_back(editModeCamera);//����������б���

	//��Ⱦģʽ���
	Object* renderingModeCamera = new Object(GetNewObjectID());
	renderingModeCamera->AddComponent<Attributes>("RenderingMode Camera","Camera", true);
	renderingModeCamera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 100.0f, -200.0f), DirectX::XMFLOAT3(DirectX::XM_PI / 6, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	renderingModeCamera->AddComponent<Camera>();
	this->objects.push_back(renderingModeCamera);//����������б���

#	pragma endregion

	#pragma region ���ɹ�Դ

	Object* directional_light1 = new Object(GetNewObjectID());
	directional_light1->AddComponent<Attributes>("Directional Light1","Light");
	directional_light1->AddComponent<Transform>(DirectX::XMFLOAT3(80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directional_light1->AddComponent<Light>(Light::LightType::Directional, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0), 1.0f, 2000.0f);
	this->objects.push_back(directional_light1);
	lights.push_back(directional_light1);

	Object* directional_light2 = new Object(GetNewObjectID());
	directional_light2->AddComponent<Attributes>("Directional Light2", "Light");
	directional_light2->AddComponent<Transform>(DirectX::XMFLOAT3(-80, 80, -80), DirectX::XMFLOAT3(DirectX::XM_PI / 4, -DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));
	directional_light2->AddComponent<Light>(Light::LightType::Directional, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0), 1.0f, 2000.0f);
	this->objects.push_back(directional_light2);
	lights.push_back(directional_light2);

	#pragma endregion

	#pragma region ������պ�

	//Object* skybox = new Object(GetNewObjectID());
	//skybox->AddComponent<Attributes>("DefaultSkybox", "Skybox");
	//skybox->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(500, 500, 500));

	////��պ������ļ�·��
	//std::vector<std::wstring> skyboxTextureList = {
	//	L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Left.png", L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Right.png",
	//		L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Top.png", L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Bottom.png",
	//		L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Front.png", L"Assets\\Textures\\NightSkybox\\UrbanNightSky_Back.png" };

	//skybox->AddComponent<Skybox>(device, deviceContext, skyboxTextureList);

	////Ĭ����պ���ɫ��
	//std::string skyboxShaderFilePaths[] = {
	//	shaderFolderPath + "DefaultSkybox.hlsl"
	//};

	//skybox->AddComponent<MaterialManager>(device, skyboxShaderFilePaths, 1);

	//this->objects.push_back(skybox);

	#pragma endregion

	#pragma region ���ɳ�������

	//������
	Object* robot = new Object(GetNewObjectID());
	robot->AddComponent<Attributes>("Robot", "Object");
	robot->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));//���Transform���
	robot->AddComponent<MeshRenderer>(device, deviceContext, "Assets\\Models\\Robot Kyle.fbx");//���MeshRender���
	
	std::string shaderFilePaths[] = {
		shaderFolderPath + "PBRTest.hlsl",
		shaderFolderPath + "PBRTest.hlsl"
	};

	robot->AddComponent<MaterialManager>(device, shaderFilePaths, 2);//���MaterialManager���

	this->objects.push_back(robot);

	//���ɵ���ƽ��
	Object* ground = new Object(GetNewObjectID());
	ground->AddComponent<Attributes>("Ground");
	ground->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(DirectX::XM_PI / 2, 0, 0), DirectX::XMFLOAT3(10, 10, 10));//���Transform���
	ground->AddComponent<MeshRenderer>(device, deviceContext, "Assets\\Models\\Quad.fbx");//���MeshRender���
	ground->AddComponent<MaterialManager>(device, shaderFilePaths, 1);//���MaterialManager���

	this->objects.push_back(ground);

	#pragma endregion

	return true;
}

UINT SceneManager::GetNewObjectID()
{
	return objects.size();
}
