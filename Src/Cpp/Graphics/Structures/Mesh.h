#pragma once

#include<vector>

#include"VertexTypes.h"
#include"IndexBuffer.h"
#include"VertexBuffer.h"



class Mesh 
{

public:
	Mesh(ID3D11Device* device, std::vector<Vertex3D> vertices, std::vector<DWORD> indices);

	VertexBuffer<Vertex3D> GetVertexBuffer();
	IndexBuffer<DWORD> GetIndexBuffer();

private:
	VertexBuffer<Vertex3D> m_vertexBuffer; //¶¥µã»º³å
	IndexBuffer<DWORD> m_indexBuffer; //Ë÷Òý»º³å
};