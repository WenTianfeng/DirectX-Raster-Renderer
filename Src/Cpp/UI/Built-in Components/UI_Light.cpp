#include"UI_Light.h"

Light::LightType UI_Light::light_Type = Light::LightType::Directional;
std::vector<std::string> UI_Light::lightTypeNames = { "Directional","Point","Spot" };
std::string UI_Light::selectedLightType = "Directional";
float UI_Light::light_Color[4] = { 0 };
float UI_Light::light_Intensity = 0;
float UI_Light::light_Range = 0;
float UI_Light::light_SpotAngle = 60.0f;


void UI_Light::Render()
{
	if (!ImGui::CollapsingHeader("LIGHT"))
	{
		//绘制光照类型选择框
		if (ImGui::BeginCombo("Light Type", selectedLightType.c_str(), 0))
		{
			static int lightTypeSelected = -1;

			//绘制光照类型选择框
			for (unsigned int n = 0; n < lightTypeNames.size(); n++)
			{
				if (ImGui::Selectable(lightTypeNames[n].c_str(), lightTypeSelected == n))
				{
					lightTypeSelected = n;
					selectedLightType = lightTypeNames[n];

					if (selectedLightType == "Directional")
					{
						light_Type = Light::LightType::Directional;
					}
					else if (selectedLightType == "Point")
					{
						light_Type = Light::LightType::Point;
					}
					else
					{
						light_Type = Light::LightType::Spot;
					}
				}
			}

			ImGui::EndCombo();
		}

		//绘制光照颜色选择
		ImGui::ColorEdit4("Light Color", light_Color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
		ImGui::DragFloat("Intensity", &light_Intensity, 0.1f, 0.0f, 100.0f, "%.3f");
		if (light_Type != Light::LightType::Directional)
		{
			ImGui::DragFloat("Range", &light_Range, 0.8f, 0.0f, 2000.0f, "%.3f");
		}

		if (light_Type == Light::LightType::Spot)
		{
			ImGui::DragFloat("Angle", &light_SpotAngle, 0.2f, 0.0f, 179.0f, "%.3f");
		}
	}
}

void UI_Light::UpdateValues(Light::LightType lightType, DirectX::XMFLOAT4 lightColor, float intensity, float range, float spotAngle)
{
	light_Type = lightType;

	light_Color[0] = lightColor.x;
	light_Color[1] = lightColor.y;
	light_Color[2] = lightColor.z;
	light_Color[3] = lightColor.w;

	light_Intensity = intensity;
	light_Range = range;
	light_SpotAngle = spotAngle;

	switch (lightType)
	{
	case Light::LightType::Directional:
		selectedLightType = "Directional";
		break;
	case Light::LightType::Spot:
		selectedLightType = "Spot";
		break;
	case Light::LightType::Point:
		selectedLightType = "Point";
		break;
	}

}