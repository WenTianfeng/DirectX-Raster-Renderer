#include"Hierarchy.h"
#include <stdio.h>



int Hierarchy::selected = 0;

void Hierarchy::Render()
{



	if (ImGui::Begin("HIERARCHY"))
	{

        //ImGui::BeginChild("left pane", ImVec2(150, 0), true);

        for (int i = 0; i < 10; i++)
        {
            char label[128];
            sprintf(label, "MyObject %d", i);

            if (ImGui::Selectable(label, selected == i))
            {
                selected = i;
            }
        }

        //ImGui::EndChild();
	}

	ImGui::End();
}
