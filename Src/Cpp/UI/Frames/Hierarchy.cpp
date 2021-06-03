#include"Hierarchy.h"

#include"..\..\Components\Attributes.h"

int Hierarchy::selected = -1;

void Hierarchy::Render(int windowWidth, int windowHeight)
{    
    ImGuiWindowFlags hierarchyWindowFlags = ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("HIERARCHY", NULL, ImGuiWindowFlags_NoMove))
	{
        for (UINT i = 0; i < m_sceneManager->GetObjects().size(); i++)
        {
            if (m_sceneManager->GetObjects()[i]->GetComponent<Attributes>()->IsDisplayInHierarchy())
            {
                char label[128];
                std::string objectName(m_sceneManager->GetObjects()[i]->GetComponent<Attributes>()->ObjectName());

                objectName.copy(label, objectName.size(), 0);
                *(label + objectName.size()) = '\0';

                if (ImGui::Selectable(label, selected == i))
                {
                    selected = i;
                }
            }

        }
	}

    ImGui::End();

    ImGui::SetWindowPos("HIERARCHY", ImVec2(0, 20));
    ImGui::SetWindowSize("HIERARCHY", ImVec2(250, windowHeight-20.0f));
}


void Hierarchy::Initialize(SceneManager* sceneManager)
{
    this->m_sceneManager = sceneManager;
}
