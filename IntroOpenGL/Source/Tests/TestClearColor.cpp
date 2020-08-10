#include "TestClearColor.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test {

	ClearColor::ClearColor()
		: m_Color{ 1.0f, 1.0f, 1.0f, 1.0f }
	{
	}

	ClearColor::~ClearColor()
	{
	}

	void ClearColor::OnUpdate(float deltaTime)
	{
	}

	void ClearColor::OnRender()
	{
		GLCall(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void ClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_Color);
	}

}