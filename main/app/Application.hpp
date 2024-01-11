#pragma once

#include <iostream>

#include "dep/imgui/imgui.h"
#include "dep/imgui/imgui_impl_sdl2.h"
#include "dep/imgui/imgui_impl_sdlrenderer2.h"

#include "src/SDL2/SDL.hpp"
#include "src/Scene.hpp"

class Application {
private:
	static SDL::Manager* renderManager;
	static Scene* scene;
	static ImGuiIO* io;

private:
	static void renderSceneSettings();
	static void renderFrameRateBox();
	static void renderProjectionSettings();
	static void renderViewSettings();
	static void renderUserInterface();

public:
	static void initializeApplication();
	static void runApplication();
	static void closeApplication();
};
