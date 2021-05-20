#include "UserInterface.h"
#include"..\..\Tools\FileManager.h"

bool UserInterface::Initialize(HWND hwnd, ID3D11Device* device,ID3D11DeviceContext* deviceContext, SceneManager* sceneManager)
{
	m_sceneManager = sceneManager;

	InitializeImGui(hwnd, device, deviceContext);
	InitializeModules(sceneManager);

	return true;
}

void UserInterface::Render()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//=======���ݻ���=======

	this->m_topToolbar->Render();
	this->m_hierarchy->Render();
	this->m_inspector->Render();

	//======���ݻ��ƽ���=====
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
	//��������
	ImFont* defaultFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 16.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

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

