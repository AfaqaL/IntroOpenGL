#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>  
#include <string>
#include <sstream>

#include"Renderer.h"
#include"IndexBuffer.h"
#include"VertexBuffer.h"
#include"VertexBufferLayout.h"
#include"VertexArray.h"
#include"Shader.h"
#include"Texture.h"

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

static void ChangeColor(float* red_val, int& idx, float& diff) {
	if (red_val[idx] > 1.0f) {
		++idx;
		if (idx % 3 == 0) {
			idx = 0;
			diff = -0.02f;
		}
	}
	else if (red_val[idx] < 0.0f) {
		++idx;
		if (idx % 3 == 0) {
			idx = 0;
			diff = 0.02f;
		}
	}
	red_val[idx] += diff;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Baro Lasha", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR!\n";
	std::cout << glGetString(GL_VERSION) << std::endl;

#pragma region My stuff after initialization
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

#pragma region crown points

		float crown_pos[] = {
			-0.3f, 0.5f, //left-down 0
			 0.0f, 0.95f, //top-mid 1
			 0.3f, 0.5f, //right-down 2
			-0.5f, 0.85f, // left-top 3
			 0.5f, 0.85f //top_right 4
		};

		unsigned int crown_indicies[] = {
			0, 3, 2,
			0, 1, 2,
			0, 4, 2
		};

		float red_val[] = {
			0.0f, 0.0f, 0.0f
		};
		int idx = 0;
		float diff = 0.02f;
		
#pragma endregion
		
		/* blending */
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexArray cr_va;

		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
		
		VertexBuffer cr_vb(crown_pos, 5 * 2 * sizeof(float));
		VBufferLayout cr_layout;
		cr_layout.Push<float>(2);
		cr_va.AddBuffer(cr_vb, cr_layout);
		
		IndexBuffer cr_ib(crown_indicies, 9);
		
		Shader cr_shader("Resources/Shaders/Crown.shader");
		cr_shader.Bind();

		IndexBuffer ib(indicies, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		

		Shader shader("Resources/Shaders/Basic.shader");
		shader.Bind();
		Texture texture("Resources/Textures/nino.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);
		
		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		Renderer renderer;

		/* >>>>>>>>>>>>>>>>>>>>>
			!!!!! Setting IMGUI !!!!
			<<<<<<<<<<<<<<<<<<<< */

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init((const char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
		ImGui::StyleColorsDark();

		glm::vec3 translation = glm::vec3(200, 200, 0);
		glm::vec3 translation2 = glm::vec3(400, 200, 0);

		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();

			ImGui::NewFrame();

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.SetUniformMat4("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translation2);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.SetUniformMat4("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}

			vb.Bind();
			
			cr_shader.Bind();
			cr_shader.SetUniform4f("u_Color", clear_color.x, 
				clear_color.y, clear_color.z, clear_color.w);
			cr_vb.Bind();
			renderer.Draw(cr_va, cr_ib, cr_shader);

			ChangeColor(red_val, idx, diff);

			{
				//static float f = 0.0f;
				//static int counter = 0;

				//ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				//ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat3("Translation A", &translation.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat3("Translation B", &translation2.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				//	counter++;
				//ImGui::SameLine();
				//ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				//ImGui::End();
			}
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}

#pragma endregion
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}