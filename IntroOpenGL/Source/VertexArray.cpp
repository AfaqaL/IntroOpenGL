#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VBufferLayout& vbl)
{
	Bind();
	vb.Bind();
	const auto& elems = vbl.GetElemenets();

	unsigned int offset = 0;
	for (unsigned int i = 0; i < elems.size(); i++)
	{
		const auto& elem = elems[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, elem.count, elem.type,
			elem.normalized, vbl.GetStride(), (const void*)offset);
		offset += elem.count * VB_Element::GetTypeSize(elem.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
