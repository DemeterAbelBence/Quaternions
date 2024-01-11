#include <glm.hpp>

struct Quaternion {
	float w;
	float x;
	float y;
	float z;

	Quaternion();
	Quaternion(float _w, float _x, float _y, float _z);

	glm::vec3 vector() const { return glm::vec3(x, y, z); }
	float scalar() const { return w; }

	Quaternion conjugate(void);
	float length();
	Quaternion normalize();

	void operator=(Quaternion q);
	Quaternion operator+=(Quaternion q);
	Quaternion operator+(Quaternion q);
	Quaternion operator*(float scalar);
	Quaternion operator*(Quaternion q);
};