#pragma once

#include"..\ImGui\imgui.h"
#include"..\ImGui\imgui_impl_dx11.h"
#include"..\ImGui\imgui_impl_win32.h"

#include"..\..\Logic\SceneManager.h"
#include"..\..\Logic\ObjectScripts\CameraControl.h"

class TopToolbar
{
public:
	static bool showStyleSettingPanel;
	static bool showRealTimeRenderingSettingPanel;
	static bool showOfflineRenderingSettingPanel;

public:
	void Render();

private:

	void ShowStyleSettingPanel(bool* p_open);
	void ShowRealTimeRenderingSettingPanel(bool* p_open);
	void ShowOfflineRenderingSettingPanel(bool* p_open);
};