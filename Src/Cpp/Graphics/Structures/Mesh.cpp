#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, std::vector<Vertex3D> vertices, std::vector<DWORD> indices)
{
	this->m_vertexBuffer.Initialize(device, vertices.data(), vertices.size());

	this->m_indexBuffer.Initialize(device, indices.data(), indices.size());
}

VertexBuffer<Vertex3D> Mesh::GetVertexBuffer()
{
	return this->m_vertexBuffer;
}

IndexBuffer<DWORD> Mesh::GetIndexBuffer()
{
	return this->m_indexBuffer;
}
