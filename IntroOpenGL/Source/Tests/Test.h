#pragma once

#include <vector>
#include <functional>
#include <string>

namespace test {

	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu :public Test {
	public:
		TestMenu(Test*& currentTest);

		void OnImGuiRender() override;

		template<typename TestClassType>
		void RegisterTest(const std::string& name) 
		{
			m_Tests.push_back(std::make_pair(name, []() { return new TestClassType(); }));
		}
	private:
		Test*& m_CurrTest;
		std::vector<std::pair<std::string, std::function<Test * ()> > > m_Tests;
	};

}