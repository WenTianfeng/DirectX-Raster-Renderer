#include "TopToolbar.h"

bool TopToolbar::showStyleSettingPanel = false;
bool TopToolbar::showRealTimeRenderingSettingPanel = false;
bool TopToolbar::showOfflineRenderingSettingPanel = false;

void TopToolbar::Render()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			if (ImGui::MenuItem("New")) 
			{
			
			}
			if (ImGui::MenuItem("Open", "Ctrl+O")) 
			{
			
			}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) 
			{
			
			}
			if (ImGui::MenuItem("Save As..")) 
			{
				//打开存储场景面板
			}

			ImGui::Separator();
			if (ImGui::BeginMenu("Options"))
			{
				static bool enabled = true;
				ImGui::MenuItem("Enabled", "", &enabled);
				ImGui::BeginChild("child", ImVec2(0, 60), true);
				for (int i = 0; i < 10; i++)
					ImGui::Text("Scrolling Text %d", i);
				ImGui::EndChild();
				static float f = 0.5f;
				static int n = 0;
				ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
				ImGui::InputFloat("Input", &f, 0.1f);
				ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Quit", "Alt+F4")) {}

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

		if (ImGui::BeginMenu("Preference"))
		{
			ImGui::MenuItem("Style Setting", NULL, &showStyleSettingPanel);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Object"))
		{
			if (ImGui::BeginMenu("Create 3D Object"))
			{
				ImGui::MenuItem("Plane");
				ImGui::MenuItem("Box");
				ImGui::MenuItem("Sphere");
				ImGui::MenuItem("Cylinder");
				ImGui::MenuItem("Capsule");

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Load Model", NULL))
			{
				//打开文件加载窗口
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Rendering"))
		{
			ImGui::MenuItem("Real-Time Rendering Setting", NULL, &showRealTimeRenderingSettingPanel);
			ImGui::MenuItem("Offline Rendering Setting", NULL, &showOfflineRenderingSettingPanel);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Examine Mode", NULL))
			{
				SceneManager::mainCamera->GetComponent<CameraControl>()->SetExamineMode(true);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Source Code Link", NULL)) 
			{
				
			}
			if (ImGui::MenuItem("Documentation", NULL))
			{

			}
			if (ImGui::MenuItem("Author Infomation", NULL))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	//如果打开样式设置面板
	if (showStyleSettingPanel)
	{
		ShowStyleSettingPanel(&showStyleSettingPanel);
	}

	if (showRealTimeRenderingSettingPanel)
	{
		ShowRealTimeRenderingSettingPanel(&showRealTimeRenderingSettingPanel);
	}
}


void TopToolbar::ShowStyleSettingPanel(bool* p_open)
{
	if (ImGui::Begin("Style Setting", p_open))
	{
		ImGui::End();
	}
}


void TopToolbar::ShowRealTimeRenderingSettingPanel(bool* p_open)
{
	if (ImGui::Begin("Real-Time Rendering Setting", p_open))
	{

		ImGui::End();
	}
}

void TopToolbar::ShowOfflineRenderingSettingPanel(bool* p_open)
{
	if (ImGui::Begin("Offline Rendering Setting", p_open))
	{

		ImGui::End();
	}
}
