#include "Application.hpp"

SDL::Manager* Application::renderManager = new SDL::Manager("Pellets", 1000, 800);;
Scene* Application::scene = new Scene();
ImGuiIO* Application::io = nullptr;

void Application::renderFrameRateBox() {
	ImGui::Begin("Pellets performance:");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
	ImGui::End();
}

void Application::renderSceneSettings() {
	ImGui::Begin("Scene settings");

	if (ImGui::Button("Next pellet        "))
		scene->incrementPelletId();
	if (ImGui::Button("Set as axis point A"))
		scene->setSelectedAsAxisA();
	if (ImGui::Button("Set as axis point B"))
		scene->setSelectedAsAxisB();

	ImGui::End();
}

void Application::renderProjectionSettings() {
	static float fov = 0.5f, near = 0.0f, far = 5.0f, top = 1.0f,
		bottom = -1.0f, right = -1.0f, left = 2.0f;
	static const float fov_limit = 2.0f;
	static const float clip_limit = 5.0f;
	static const char* fov_precision = "%.1f";
	static const char* clip_precision = "%.1f";

	ImGui::Begin("Projection settings");
	ImGui::SliderFloat("fov", &fov, -fov_limit, fov_limit, fov_precision, 0);
	ImGui::SliderFloat("near", &near, -clip_limit, clip_limit, clip_precision, 0);
	ImGui::SliderFloat("far", &far, -clip_limit, clip_limit, clip_precision, 0);
	ImGui::SliderFloat("top", &top, -clip_limit, clip_limit, clip_precision, 0);
	ImGui::SliderFloat("bottom", &bottom, -clip_limit, clip_limit, clip_precision, 0);
	ImGui::SliderFloat("right", &right, -clip_limit, clip_limit, clip_precision, 0);
	ImGui::SliderFloat("left", &left, -clip_limit, clip_limit, clip_precision, 0);
	if (ImGui::Button("Reset to default")) {
		near = 0.0f; far = 5.0f; top = 1.0f;
		bottom = -1.0f; right = -1.0f; left = 2.0f;
	}
	ImGui::End();
	
	Camera* camera = scene->getCamera();
	camera->setFieldOfView(fov);
	camera->setNear(near);
	camera->setFar(far);
	camera->setTop(top);
	camera->setBottom(bottom);
	camera->setLeft(left);
	camera->setRight(right);
	camera->resetProjection();
}

void Application::renderViewSettings() {
	static float eye[3] = { 0.0f, 0.0f, 0.0f };
	static float target[3] = { 0.0f, 0.0f, 1.0f };
	static float up[3] = { 0.0f, 1.0f, 0.0f };
	static float limit = 3.0f;
	static const char* precision = "%.1f";

	ImGui::Begin("Camera settings");
	ImGui::SliderFloat3("eye", &eye[0], -limit, limit, precision, 0);
	ImGui::SliderFloat3("target", &target[0], -limit, limit, precision, 0);
	ImGui::SliderFloat3("up", &up[0], -limit, limit, precision, 0);
	if (ImGui::Button("Reset to default")) {
		eye[0] = 0.0f; eye[1] = 0.0f; eye[2] = 0.0f;
		target[0] = 0.0f; target[1] = 0.0f; target[2] = 1.0f;
		up[0] = 0.0f; up[1] = 1.0f; up[2] = 0.0f;
	}
	ImGui::End();

	Camera* camera = scene->getCamera();
	camera->setEye(glm::vec3(eye[0], eye[1], eye[2]));
	camera->setTarget(glm::vec3(target[0], target[1], target[2]));
	camera->setUp(glm::vec3(up[0], up[1], up[2]));
	camera->resetView();
}

void Application::renderUserInterface() {
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	renderFrameRateBox();
	renderSceneSettings();
	renderProjectionSettings();
	renderViewSettings();

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void Application::initializeApplication() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO();  (void)*io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(renderManager->getWindow(), renderManager->getRenderer());
	ImGui_ImplSDLRenderer2_Init(renderManager->getRenderer());

	scene->getEvent()->setGuiEventHandler(&ImGui_ImplSDL2_ProcessEvent);
}

void Application::runApplication() {
	const int FPS = 60;
	const float frameTime = 1000 / FPS;
	float previousTime = 0;
	float currentTime = 0;
	float deltaTime = 0;

	bool quit = false;
	while (!quit) {
		previousTime = currentTime;

		renderManager->clearFrame();
		quit = scene->update(*renderManager);
		scene->draw(*renderManager);
		renderUserInterface();
		renderManager->renderFrame();

		currentTime = renderManager->getTime();
		deltaTime = currentTime - previousTime;
		if (frameTime - deltaTime > 0)
			renderManager->delay(frameTime - deltaTime);
	}
}

void Application::closeApplication() {
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete scene;
	delete renderManager;
}
