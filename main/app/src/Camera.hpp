#pragma once

#include <glm.hpp>
#include <iostream>

class Camera {

private:
	glm::vec3 m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_up = glm::vec3(0.0f, 0.0f, 0.0f);

	float m_near = 0.0f;
	float m_far = 0.0f;
	float m_top = 0.0f;
	float m_bottom = 0.0f;
	float m_left = 0.0f;
	float m_right = 0.0f;

	float field_of_view = 0.0f;

	glm::mat4 m_projection;
	glm::mat4 m_view;

private:
	glm::mat4 createOrthographic();
	glm::mat4 createPerspective();
	glm::mat4 createViewMatrix();

public:
	Camera();

	inline void setNear(float value) { m_near = value; }
	inline void setFar(float value) { m_far= value; }
	inline void setTop(float value) { m_top = value; }
	inline void setBottom(float value) { m_bottom = value; }
	inline void setLeft(float value) { m_left = value; }
	inline void setRight(float value) { m_right = value; }

	inline void setFieldOfView(float value) { field_of_view = value; }

	inline void setEye(glm::vec3 value) { m_eye = value; }
	inline void setTarget(glm::vec3 value) { m_target = value; }
	inline void setUp(glm::vec3 value) { m_up = value; }
	inline glm::vec3 getEye() const { return m_eye; }
	inline glm::mat4 getProjection() const { return m_projection; }
	inline glm::mat4 getView() const { return m_view; }

	inline void resetProjection() { m_projection = createPerspective() * createOrthographic(); }
	inline void resetView() { m_view = createViewMatrix(); }
};