#pragma once

#include"..\ImGui\imgui.h"
#include"..\ImGui\imgui_impl_dx11.h"
#include"..\ImGui\imgui_impl_win32.h"

class Hierarchy
{
public:
	static int selected;

public:
	void Render();

};