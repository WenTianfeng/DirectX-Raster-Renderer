#include "UserInterface.h"
#include"..\..\Tools\FileManager.h"

bool UserInterface::Initialize(HWND hwnd, ID3D11Device* device,ID3D11DeviceContext* deviceContext, SceneManager* sceneManager)
{
	m_sceneManager = sceneManager;

	InitializeImGui(hwnd, device, deviceContext);
	InitializeModules(sceneManager);

	return true;
}

void UserInterface::Render(int windowWidth, int windowHeight)
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//=======内容绘制=======

	this->m_topToolbar->Render();
	this->m_hierarchy->Render(windowWidth,windowHeight);
	this->m_inspector->Render(windowWidth, windowHeight);

	//======内容绘制结束=====
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void UserInterface::InitializeImGui(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);
	ImGui::StyleColorsDark();

	std::string fontPath = PresetFontFiles + "Roboto-Medium.ttf";
	//加载字体
	ImFont* defaultFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0;
	style.WindowMenuButtonPosition = -1;
}

void UserInterface::InitializeModules(SceneManager* sceneManager)
{
	this->m_hierarchy->Initialize(sceneManager);
	this->m_inspector->Initialize(sceneManager);
	this->m_topToolbar->Initialize(sceneManager);
}

