#include "UI_Attributes.h"

std::string UI_Attributes::attributes_ObjectName = "";
bool UI_Attributes::attributes_Active = true;
std::string UI_Attributes::attributes_Tag = "";

void UI_Attributes::Render()
{
	if (!ImGui::CollapsingHeader("Attributes"))
	{
		ImGui::Checkbox("Active", &attributes_Active);

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Name :");
		ImGui::SameLine();
		ImGui::Text(attributes_ObjectName.c_str());


		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Tag :");
		ImGui::SameLine();
		ImGui::Text(attributes_Tag.c_str());
	}

}

void UI_Attributes::UpdateValues(std::string objectName, bool active, std::string tag)
{
	attributes_ObjectName = objectName;
	attributes_Active = active;
	attributes_Tag = tag;
}
