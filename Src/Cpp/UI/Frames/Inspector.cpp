#include"Inspector.h"


void Inspector::Initialize()
{
	this->m_componentUIs.push_back(new UI_Transform());
}

void Inspector::Render()
{

	if (ImGui::Begin("INSPECTOR"))
	{
		for (auto componentUI : this->m_componentUIs)
		{
			componentUI->Render();
		}
	}

	ImGui::End();

}

void Inspector::RefreshUIList()
{


}
