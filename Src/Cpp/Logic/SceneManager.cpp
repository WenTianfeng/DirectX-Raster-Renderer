#include "SceneManager.h"
#include"..\Components\Skybox.h"
#include"..\Logic\ObjectScripts\CameraControl.h"

//全局静态变量初始化
Object* SceneManager::mainCamera = nullptr;
std::vector<Object*> SceneManager::lights = {};

bool SceneManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//初始化场景
	if (!InitializeScene(device,deviceContext))
	{
		return false;
	}

	return true;
}

void SceneManager::Update(float dt)
{
	//对所有场景对象进行更新
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
#pragma region 判定shader文件路径

	std::wstring shaderFolderPath;
	if (IsDebuggerPresent())
	{
#ifdef _DEBUG
		shaderFolderPath = L"Debug\\";
#else //RELEASE（待修改）
		shaderFolderPath = L"..\\..\\..\\Debug\\";
#endif // DEBUG

	}
#pragma endregion

#pragma region 生成相机

	Object* camera = new Object();
	camera->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(1, 1, 1));
	camera->AddComponent<Camera>(DirectX::XM_PI / 3, 2.0f, 0.5f, 2000.0f);
	camera->AddComponent<CameraControl>();
	mainCamera = camera;

	this->m_objects.push_back(camera);//将对象加入列表中

#pragma endregion

#pragma region 生成光源

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

#pragma region 生成天空盒

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

#pragma region 生成场景对象

	//生成立方体
	Object* cube = new Object();
	cube->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(DirectX::XM_PI / 2, DirectX::XM_PI / 4, 0), DirectX::XMFLOAT3(1, 1, 1));//添加Transform组件
	cube->AddComponent<MeshRenderer>(device, deviceContext, "Resources\\Models\\BoxBall.FBX");//添加MeshRender组件
	
	std::wstring vertexShaderFilePaths[] = {
		shaderFolderPath + L"VertexShader.cso",
		shaderFolderPath + L"VertexShader.cso"
	};

	std::wstring pixelShaderFilePaths[] = {
		shaderFolderPath + L"PixelShader_PBR.cso",
		shaderFolderPath + L"PixelShader_PBR.cso"
	};

	cube->AddComponent<MaterialManager>(device, vertexShaderFilePaths, pixelShaderFilePaths, 2);//添加MaterialManager组件

	this->m_objects.push_back(cube);

	//生成四边形
	Object* quad = new Object();
	quad->AddComponent<Transform>(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(3, 3, 3));//添加Transform组件
	quad->AddComponent<MeshRenderer>(device, deviceContext, "Resources\\Models\\Quad.FBX");//添加MeshRender组件

	std::wstring quadVertexShaderFilePaths[] = {
		shaderFolderPath + L"VertexShader.cso",
	};

	std::wstring quadPixelShaderFilePaths[] = {
		shaderFolderPath + L"PixelShader_Texture.cso",
	};

	quad->AddComponent<MaterialManager>(device, quadVertexShaderFilePaths, quadPixelShaderFilePaths, 1);//添加MaterialManager组件

	//this->m_objects.push_back(quad);

#pragma endregion

	return true;
}
