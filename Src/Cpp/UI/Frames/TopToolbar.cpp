#include "TopToolbar.h"
#include"..\..\Tools\FileManager.h"

bool TopToolbar::showStyleSettingPanel = false;
bool TopToolbar::showRealTimeRenderingSettingPanel = false;
bool TopToolbar::showOfflineRenderingSettingPanel = false;

void TopToolbar::Initialize(SceneManager* sceneManager)
{
	this->m_sceneManager = sceneManager;
}

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
			if (ImGui::MenuItem("Save", "Ctrl+S")) 
			{
				
			}
			if (ImGui::MenuItem("Save As..")) 
			{
				//打开存储场景面板
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Quit", "Alt+F4")) 
			{
				PostQuitMessage(0);
			}

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
			if (ImGui::BeginMenu("Create Preset Object"))
			{
				std::vector<std::string> presetObjects = {"Camera","Light"};

				//绘制着色器文件选择框
				for (UINT n = 0; n < presetObjects.size(); n++)
				{
					if (ImGui::MenuItem(presetObjects[n].c_str()))
					{
						this->m_sceneManager->AddObject(presetObjects[n].c_str());
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Create Model"))
			{
				std::vector<std::string> files;
				std::string filePath = PresetMeshFiles;
				std::string format = ".fbx";
				FileManager::GetFileNamesByFormat(filePath, files, format);

				for (UINT n = 0; n < files.size(); n++)
				{
					if (ImGui::MenuItem(files[n].c_str()))
					{
						std::string fileName = files[n];
						fileName.erase(fileName.size() - format.size());

						this->m_sceneManager->AddObject(fileName, PresetMeshFiles + files[n]);
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Load Model"))
			{
				std::vector<std::string> files;
				std::string filePath = CustomizedMeshFiles;
				std::string format = ".fbx";
				FileManager::GetFileNamesByFormat(filePath, files, format);

				for (UINT n = 0; n < files.size(); n++)
				{
					if (ImGui::MenuItem(files[n].c_str())) 
					{
						//加载模型至场景
						std::string fileName = files[n];
						fileName.erase(fileName.size() - format.size());

						this->m_sceneManager->AddObject(fileName, CustomizedMeshFiles + files[n]);

					}
				}

				ImGui::EndMenu();
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
				m_sceneManager->GetMainCamera()->GetComponent<CameraControl>()->SetExamineMode(true);
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
