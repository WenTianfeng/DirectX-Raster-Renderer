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
	VertexShader m_vertexShader; //������ɫ��	
	PixelShader m_pixelShader; //������ɫ��

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_dxInputLayout;//DirectX���벼�ֶ���
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dxDepthStencilState;//���ģ��״̬
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_dxBlendState;//���״̬

	ConstantBuffer<CB_VS_TransformMatrix> m_transformMatrixCB;

};