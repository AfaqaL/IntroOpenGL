#pragma once

#include"Test.h"

namespace test {

	class ClearColor : public Test {
	public:
		ClearColor();
		~ClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_Color[4];
	};
}
