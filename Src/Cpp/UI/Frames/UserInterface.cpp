#include "UserInterface.h"

bool UserInterface::Initialize(HWND hwnd,ID3D11Device* device,ID3D11DeviceContext* deviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);
	ImGui::StyleColorsDark();

	//��������
	ImFont* defaultFont = io.Fonts->AddFontFromFileTTF("Assets\\Fonts\\Roboto-Medium.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	this->m_inspector->Initialize();

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

