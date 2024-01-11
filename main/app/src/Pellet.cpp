#include "Pellet.hpp"

int Pellet::calculateDepthAlpha(const Camera& camera) const {
	glm::mat4 view = camera.getView();
	glm::vec3 position = m_quaternion->vector();
	glm::vec4 final_position = view * glm::vec4(position, 1.0f);

	float depth = final_position.z;
	float d = glm::clamp(abs(depth - min_depth) / (max_depth - min_depth), 0.0f, 1.0f);

	return glm::mix(255, 10, d);
}

int Pellet::getScreenRadius(const SDL::Manager& manager, const Camera& camera) const {
	glm::mat4 view = camera.getView();
	glm::vec3 vector = m_quaternion->vector();
	glm::vec4 p = glm::vec4(vector, 1.0f) * view;

	int width = manager.getWindowWidth();
	int height = manager.getWindowHeight();
	float scalar = m_quaternion->scalar();
	float scaled_radius = radius_factor * scalar;
	float radius = scaled_radius / (p.z != 0 ? p.z : 1.0f);
	return static_cast<unsigned int>(radius);
}

Pellet::Pellet() {
    m_quaternion = nullptr;
	m_color = nullptr;
	m_outlined = false;
	m_id = 0;
}

Pellet::Pellet(Quaternion* quaternion, SDL::Color* color, unsigned int id) {
	m_quaternion = quaternion;
    m_color = color;
	m_outlined = false;
	m_id = id;
}

glm::vec2 Pellet::getScreenPosition(const SDL::Manager& manager, const Camera& camera) const {
	glm::mat4 projection = camera.getProjection();
	glm::mat4 view = camera.getView();
	glm::vec3 vector = m_quaternion->vector();

	glm::vec4 p = glm::vec4(vector, 1.0f) * view * projection;
	p.x = p.x / (p.z != 0.0f ? p.z : 1.0f);
	p.y = p.y / (p.z != 0.0f ? p.z : 1.0f);

	int width = manager.getWindowWidth();
	int height = manager.getWindowHeight();
	float x = p.x * (width / 2) + width / 2;
	float y = p.y * (height / 2) + height / 2;

	return glm::vec2(x, y);
}

void Pellet::draw(const SDL::Manager& manager, const Camera& camera) const {
	glm::vec2 screen_position = getScreenPosition(manager, camera);
	int radius = getScreenRadius(manager, camera);
	int depth_color = calculateDepthAlpha(camera);

	if (m_outlined) {
		manager.drawCircle(screen_position.x, screen_position.y,
			radius + 5, SDL::Color(255, 255, 255, depth_color));
	}
	SDL::Color c(m_color->red, m_color->green, m_color->blue, depth_color);
	manager.drawCircle(screen_position.x, screen_position.y, radius, c);
}

void Pellet::translate(glm::vec3 t) {
	Quaternion q = Quaternion(0.0f, t.x, t.y, t.z);
	*m_quaternion += q;
}

void Pellet::rotate(glm::vec3 pivot, glm::vec3 axis, float angle) {
	glm::vec3 p = glm::vec3(m_quaternion->x, m_quaternion->y, m_quaternion->z);
	glm::vec3 v = p - pivot;

	angle *= 3.14159f;
	Quaternion r = Quaternion(
		cos(angle / 2),
		axis.x * sin(angle / 2),
		axis.y * sin(angle / 2),
		axis.z * sin(angle / 2)
	);
	Quaternion q = Quaternion(0.0f, v.x, v.y, v.z);
	r = r.normalize();
	q = r * q * r.conjugate();

	v = glm::vec3(q.x, q.y, q.z);
	p = pivot + v;
	*m_quaternion = Quaternion(m_quaternion->w, p.x, p.y, p.z);
}

Pellet::~Pellet() {
	delete m_quaternion;
	delete m_color;
}
