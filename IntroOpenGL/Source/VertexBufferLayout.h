#pragma once

#include<vector>
#include"Renderer.h"

struct VB_Element {
	unsigned int count;
	unsigned int type;
	unsigned char normalized;

	static unsigned int GetTypeSize(unsigned int type) {
		switch (type) {
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VBufferLayout {
private:
	std::vector<VB_Element> m_Elements;
	unsigned int m_Stride;
public:
	VBufferLayout():
		m_Stride(0){}

	template<typename T>
	void Push(unsigned int count) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count) {
		m_Elements.push_back({ count, GL_FLOAT, GL_FALSE });
		m_Stride += VB_Element::GetTypeSize(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
		m_Stride += VB_Element::GetTypeSize(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
		m_Stride += VB_Element::GetTypeSize(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VB_Element>& GetElemenets() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};
