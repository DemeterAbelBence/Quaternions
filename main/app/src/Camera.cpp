#include "Camera.hpp"

glm::mat4 Camera::createOrthographic() {
    float X = -(m_right + m_left) / (m_right - m_left);
    float Y = -(m_top + m_bottom) / (m_top - m_bottom);
    float Z = -(m_far + m_near) / (m_far - m_near);

    glm::mat4 orthographic;
    orthographic[0] = glm::vec4(2.0f / (m_right - m_left), 0.0f, 0.0f, X);
    orthographic[1] = glm::vec4(0.0f, 2.0f / (m_top - m_bottom), 0.0f, Y);
    orthographic[2] = glm::vec4(0.0f, 0.0f, 1.0f, Z);
    orthographic[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    return orthographic;
}

glm::mat4 Camera::createPerspective() {
    float A = m_far / (m_far - m_near);
    float B = -m_far * m_near / (m_far - m_near);
    float angle = 3.14159f * field_of_view / 2;
    float fov = glm::tan(angle);
    float scale = 1.0f / (fov != 3.14159f / 2 ? fov : 1.0f);

    glm::mat4 p1;
    p1[0] = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    p1[1] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    p1[2] = glm::vec4(0.0f, 0.0f, A, B);
    p1[3] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

    glm::mat4 p2;
    p2[0] = glm::vec4(scale, 0.0f, 0.0f, 0.0f);
    p2[1] = glm::vec4(0.0f, scale, 0.0f, 0.0f);
    p2[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    p2[3] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

    return p1 * p2;
}

glm::mat4 Camera::createViewMatrix() {
    glm::vec3 D = glm::normalize(m_target - m_eye);
    glm::vec3 R = glm::normalize(glm::cross(m_up, D));
    glm::vec3 U = glm::normalize(glm::cross(D, R));

    glm::mat4 rotation;
    rotation[0] = glm::vec4(R, 0.0f);
    rotation[1] = glm::vec4(U, 0.0f);
    rotation[2] = glm::vec4(D, 0.0f);
    rotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 translation;
    translation[0] = glm::vec4(1.0f, 0.0f, 0.0f, -m_eye.x);
    translation[1] = glm::vec4(0.0f, 1.0f, 0.0f, -m_eye.y);
    translation[2] = glm::vec4(0.0f, 0.0f, 1.0f, -m_eye.z);
    translation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    return rotation * translation;
}

Camera::Camera() {}


