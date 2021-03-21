#pragma once

#include"VertexTypes.h"

#include"IndexBuffer.h"
#include"VertexBuffer.h"



class Mesh 
{

public:
	Mesh(ID3D11Device* device, std::vector<Vertex3D> vertices,std::vector<DWORD> indices)
	{
		this->m_vertexBuffer.Initialize(device, vertices.data(), vertices.size());

		this->m_indexBuffer.Initialize(device, indices.data(), indices.size());
	}

private:
	VertexBuffer<Vertex3D> m_vertexBuffer; //���㻺��
	IndexBuffer<DWORD> m_indexBuffer; //��������
};