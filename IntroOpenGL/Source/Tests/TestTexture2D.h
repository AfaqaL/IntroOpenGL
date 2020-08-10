#pragma once

#include"Test.h"


#include "VertexBuffer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"

#include <memory>

namespace test {

	class Texture2D : public Test {
	public:
		Texture2D();
		~Texture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;
	};
}
