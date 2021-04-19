#pragma once

#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）
#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）

#include"ImGui\imgui.h"
#include"ImGui\imgui_impl_dx11.h"
#include"ImGui\imgui_impl_win32.h"


class UserInterface
{
public:
	bool Initialize(HWND hwnd,ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Render();

private:
	void ShowMainMenuBar();
	void ShowHierachy();
	void ShowInspector();
	bool show_demo_window = true;

};