#pragma once

#include"Shader.h"

class Material 
{
public:
	Material(ID3D11Device* device, std::wstring vertexShaderFilePath, std::wstring pixelShaderFilePath) {
	
	}
private:
	VertexShader m_vertexShader; //顶点着色器	
	PixelShader m_pixelShader; //像素着色器


};