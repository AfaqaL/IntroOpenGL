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

#include "Tests/TestClearColor.h"
#include "Tests/TestTexture2D.h"

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

	{
		/* blending */
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		/* >>>>>>>>>>>>>>>>>>>>>
			!!!!! Setting IMGUI !!!!
			<<<<<<<<<<<<<<<<<<<< */

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init((const char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
		ImGui::StyleColorsDark();

		test::Test* currTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currTest);
		currTest = testMenu;

		testMenu->RegisterTest<test::ClearColor>("Clear Color");
		testMenu->RegisterTest<test::Texture2D>("2D Texture");

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			/* Render here */
			renderer.Clear();

			//test.OnUpdate(0.0f);
			//test.OnRender();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (currTest) {
				currTest->OnUpdate(0.0f);
				currTest->OnRender();
				ImGui::Begin("Test");
				if (currTest != testMenu) {
					if (ImGui::Button("<-")) {
						delete currTest;
						currTest = testMenu;
					}
				}
				currTest->OnImGuiRender();
				ImGui::End();
			}
			
			//test.OnImGuiRender();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}