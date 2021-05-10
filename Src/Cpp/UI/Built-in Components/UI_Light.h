#pragma once

#include<DirectXMath.h>

#include"..\..\Components\Light.h"
#include"UI_Component.h"

class UI_Light : public UI_Component
{
public:
	static Light::LightType light_Type;
	static std::vector<std::string> lightTypeNames;
	static std::string selectedLightType;
	static float light_Color[4];
	static float light_Intensity;
	static float light_Range;
	
public:
	static void Render();
	static void UpdateValues(Light::LightType lightType, DirectX::XMFLOAT4 lightColor, float intensity, float range);
};