#pragma once

#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）
#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）

#include"..\..\Logic\SceneManager.h"

#include"Inspector.h"
#include"TopToolbar.h"
#include"Console.h"

class UserInterface
{
public:

	bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, SceneManager* sceneManager);
	void Render();

private:
	void InitializeImGui(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void InitializeModules(SceneManager* sceneManager);

private:
	SceneManager* m_sceneManager = nullptr;

	Inspector* m_inspector = new Inspector();
	Hierarchy* m_hierarchy = new Hierarchy();
	TopToolbar* m_topToolbar = new TopToolbar();
	Console* m_console = new Console();

};