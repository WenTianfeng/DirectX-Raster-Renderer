#pragma once

#include"..\ImGui\imgui.h"
#include"..\ImGui\imgui_impl_dx11.h"
#include"..\ImGui\imgui_impl_win32.h"

#include"..\..\Logic\SceneManager.h"

class Hierarchy
{
public:
	static int selected;

public:
	void Initialize(SceneManager* sceneManager);
	void Render(int windowWidth, int windowHeight);

private:
	SceneManager* m_sceneManager;
};