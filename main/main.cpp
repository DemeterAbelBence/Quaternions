#pragma once

#include <iostream>

#include "app/Application.hpp"

int main(void) {
	try {
		Application::initializeApplication();
		Application::runApplication();
		Application::closeApplication();
	}
	catch (const char* exception) {
		std::cout << exception << std::endl;
	}

	return 0;
}