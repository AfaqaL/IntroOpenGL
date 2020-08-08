#pragma once
#include"VertexBuffer.h"
class VBufferLayout;

class VertexArray {
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VBufferLayout& vbl);

	void Bind() const;
	void Unbind() const;
};
