#include "Scene.hpp"

void Scene::incrementPelletId() {
	unsigned int size = m_pellets->getSize();
	if (m_pellet_id < size - 1)
		m_pellet_id++;
	else m_pellet_id = 0;
}

Pellet* Scene::getPelletById(unsigned int id) const {
	unsigned int size = m_pellets->getSize();
	for (int i = 0; i < size; ++i) {
		Pellet* p = m_pellets->getElement(i);
		if (p->getId() == id)
			return p;
	}
	throw "There is no pellet with the given ID!";
}

void Scene::connectAxisPellets(const SDL::Manager& manager) const {
	Pellet* A = nullptr;
	Pellet* B = nullptr;
	A = getPelletById(m_axis_pellet_a);
	B = getPelletById(m_axis_pellet_b);
	if (A != nullptr && B != nullptr) {
		glm::vec2 a = A->getScreenPosition(manager, *m_camera);
		glm::vec2 b = B->getScreenPosition(manager, *m_camera);
		manager.drawLine(a.x, a.y, b.x, b.y, SDL::Color(255, 255, 255, 255));
	}
}

void Scene::addNewPellet(Quaternion* q, SDL::Color* c) {
	static int pellet_id = -1;
	m_pellets->insertElement(new Pellet(q, c, ++pellet_id));
}

void Scene::generatePellets() {
	addNewPellet(new Quaternion(0.5f, 0.0f, 0.0f, 0.7f), new SDL::Color(255, 0, 0, 255));
	addNewPellet(new Quaternion(0.5f, -0.3f, 0.6f, 0.8f), new SDL::Color(0, 255, 0, 255));
	addNewPellet(new Quaternion(0.5f, 0.6f, 0.2f, 1.0f), new SDL::Color(0, 0, 255, 255));
	addNewPellet(new Quaternion(0.5f, 0.6f, 0.3f, 0.9f), new SDL::Color(255, 255, 0, 255));
	addNewPellet(new Quaternion(0.5f, -0.2f, 0.1f, 0.2f), new SDL::Color(0, 255, 255, 255));
	addNewPellet(new Quaternion(0.5f, 0.7f, 0.8f, 1.3f), new SDL::Color(255, 0, 255, 255));
}

Scene::Scene() {
	m_event = new SDL::Event();
	m_camera = new Camera();

	m_axis_pellet_a = 0;
	m_axis_pellet_b = 1;
	m_move_speed = 0.01f;
	m_rotate_speed = 0.01f;
	
	m_pellets = new Collection<Pellet>;
	generatePellets();

	unsigned int default_pellet = 0;
	m_pellet_id = default_pellet;
	m_pellets->getElement(default_pellet)->setOutlined(true);
}

bool Scene::update(const SDL::Manager& manager) {
	SDL::userInput input = m_event->getNewEvent();
	bool quit = false;
	bool transformation_ended = false;
	static glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	static float rotation_angle = 0.0f;
	switch (input) {
		case SDL::userInput::KEYDOWN_DOWN: translation.y = -m_move_speed; break;
		case SDL::userInput::KEYDOWN_UP: translation.y = m_move_speed; break;
		case SDL::userInput::KEYDOWN_LEFT: translation.x = -m_move_speed; break;
		case SDL::userInput::KEYDOWN_RIGHT: translation.x = m_move_speed; break;
		case SDL::userInput::KEYDOWN_Q: translation.z = -m_move_speed; break;
		case SDL::userInput::KEYDOWN_W: translation.z = m_move_speed; break;
		case SDL::userInput::KEYDOWN_E: rotation_angle = m_rotate_speed; break;
		case SDL::userInput::KEYDOWN_R: rotation_angle = -m_rotate_speed; break;

		case SDL::userInput::KEYUP_DOWN: transformation_ended = true; break;
		case SDL::userInput::KEYUP_UP: transformation_ended = true; break;
		case SDL::userInput::KEYUP_LEFT: transformation_ended = true; break;
		case SDL::userInput::KEYUP_RIGHT: transformation_ended = true; break;
		case SDL::userInput::KEYUP_Q: transformation_ended = true; break;
		case SDL::userInput::KEYUP_W: transformation_ended = true; break;
		case SDL::userInput::KEYUP_E: transformation_ended = true; break;
		case SDL::userInput::KEYUP_R: transformation_ended = true; break;
		case SDL::userInput::KEYUP_ESC: quit = true; break;
	}

	unsigned int size = m_pellets->getSize();
	for (int i = 0; i < size; ++i) {
		Pellet* p = m_pellets->getElement(i);
		if (p->getId() == m_pellet_id) {
			p->setOutlined(true);
			p->translate(translation);

			Quaternion* quatA = getPelletById(m_axis_pellet_a)->getQuaternion();
			Quaternion* quatB = getPelletById(m_axis_pellet_b)->getQuaternion();
			glm::vec3 vecA = glm::vec3(quatA->x, quatA->y, quatA->z);
			glm::vec3 vecB = glm::vec3(quatB->x, quatB->y, quatB->z);
			p->rotate(vecA, vecA - vecB, rotation_angle);
		}
		else p->setOutlined(false);
	}

	if (transformation_ended) {
		m_pellets->sortElements();
		translation = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation_angle = 0.0f;
	}

	return quit;
}

void Scene::draw(const SDL::Manager& manager) const {
	connectAxisPellets(manager);
	unsigned int size = m_pellets->getSize();
	for (int i = 0; i < size; ++i)
		m_pellets->getElement(i)->draw(manager, *m_camera);
}

Scene::~Scene() {
	delete m_event;
	delete m_pellets;
	delete m_camera;
}
