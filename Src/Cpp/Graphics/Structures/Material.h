#pragma once

#include"Shader.h"
#include"ConstantBuffer.h"
#include"ConstantBufferTypes.h"


class Material 
{
public:
	Material(ID3D11Device* device, std::wstring vertexShaderFilePath, std::wstring pixelShaderFilePath);

	VertexShader GetVertexShader();
	PixelShader GetPixelShader();
	ID3D11InputLayout* GetInputLayout();
	ID3D11DepthStencilState* GetDepthStencilState();
	ID3D11BlendState* GetBlendState();

	ConstantBuffer<CB_VS_TransformMatrix>& GetConstantBuffer_TransformMatrix();

private:
	VertexShader m_vertexShader; //顶点着色器	
	PixelShader m_pixelShader; //像素着色器

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_dxInputLayout;//DirectX输入布局对象
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dxDepthStencilState;//深度模板状态
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_dxBlendState;//混合状态

	ConstantBuffer<CB_VS_TransformMatrix> m_transformMatrixCB;

};