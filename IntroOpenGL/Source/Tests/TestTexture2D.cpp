#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "VertexBuffer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	Texture2D::Texture2D()
		: m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f
		};

		unsigned int indicies[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		m_VAO = std::make_unique<VertexArray>();

		m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		
		VBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(indicies, 6);

		m_Texture = std::make_unique<Texture>("Resources/Textures/nino.png");

		m_Shader = std::make_unique<Shader>("Resources/Shaders/Basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);
	}
	Texture2D::~Texture2D()
	{
	}
	void Texture2D::OnUpdate(float deltaTime)
	{
	}
	void Texture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		
		m_Texture->Bind();
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
		}
	}
	void Texture2D::OnImGuiRender()
	{
		ImGui::SliderFloat("Translation A(H)", &m_TranslationA.x, 0.0f, 960.0f);  
		ImGui::SliderFloat("Translation A(V)", &m_TranslationA.y, 0.0f, 540.0f);  
		ImGui::SliderFloat("Translation B(H)", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::SliderFloat("Translation B(V)", &m_TranslationB.y, 0.0f, 540.0f);
		ImGui::Text("Application avarage: %.3f ms/frame (%1.f FPS)",
			1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}