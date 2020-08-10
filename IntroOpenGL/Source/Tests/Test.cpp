#include"Test.h"

#include "imgui/imgui.h"
namespace test {
	TestMenu::TestMenu(Test*& currentTest)
		: m_CurrTest(currentTest)
	{
	}
	void TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str())) {
				m_CurrTest = test.second();
			}
		}
	}
}