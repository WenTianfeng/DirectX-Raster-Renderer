#include"Hierarchy.h"
#include"..\..\Logic\SceneManager.h"
#include"..\..\Components\Attributes.h"

int Hierarchy::selected = -1;

void Hierarchy::Render()
{

	if (ImGui::Begin("HIERARCHY"))
	{
        for (UINT i = 0; i < SceneManager::objects.size(); i++)
        {
            char label[128];
            std::string objectName(SceneManager::objects[i]->GetComponent<Attributes>()->ObjectName());

            objectName.copy(label, objectName.size(), 0);
            *(label + objectName.size()) = '\0';

            if (ImGui::Selectable(label, selected == i))
            {
                selected = i;
            }
        }
	}

    ImGui::End();
}
