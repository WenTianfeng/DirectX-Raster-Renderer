#pragma once

#include"Shader.h"

class Material 
{
public:
	Material(ID3D11Device* device, std::wstring vertexShaderFilePath, std::wstring pixelShaderFilePath) {
	
	}
private:
	VertexShader m_vertexShader; //������ɫ��	
	PixelShader m_pixelShader; //������ɫ��


};