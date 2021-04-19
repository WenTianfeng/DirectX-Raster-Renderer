#include "UserInterface.h"

bool UserInterface::Initialize(HWND hwnd,ID3D11Device* device,ID3D11DeviceContext* deviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);
	ImGui::StyleColorsDark();

	//加载字体
	ImFont* defaultFont = io.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Roboto-Medium.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	return true;
}

void UserInterface::Render()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//=======内容绘制=======

	//显示上方工具栏
	ShowMainMenuBar();

	//显示场景层次结构
	ShowHierachy();

	//显示对象检查器
	ShowInspector();

	//======内容绘制结束=====
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

}

void UserInterface::ShowMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Object"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Rendering"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

}

void UserInterface::ShowHierachy()
{
	if (ImGui::Begin("HIERACHY", &show_demo_window))
	{
		ImGui::End();
	}

}

void UserInterface::ShowInspector()
{
	if (ImGui::Begin("INSPECTOR", &show_demo_window))
	{
		ImGui::End();
	}

}
