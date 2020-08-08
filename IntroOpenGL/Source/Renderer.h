#pragma once

#include <GL/glew.h>
#include"IndexBuffer.h"
#include"VertexArray.h"
#include"Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* func, const char* fileName, int line);


class Renderer {
public:
	void Clear();
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};