#include <iostream>
#include <glm.hpp>

#include "Quaternion.hpp"
#include "SDL2/SDL.hpp"
#include "Camera.hpp"

class Pellet {

private:
	Quaternion* m_quaternion;
	SDL::Color* m_color;

	bool m_outlined;
	unsigned int m_id;

private:
	const float radius_factor = 50.0f;
	const float max_depth = 2.0f;
	const float min_depth = 0.0f;

private:
	int calculateDepthAlpha(const Camera& camera) const;
	int getScreenRadius(const SDL::Manager& manager, const Camera& camera) const;

public:
	Pellet();
	Pellet(Quaternion* quaternion, SDL::Color* color, unsigned int id);

	inline void setId(unsigned int value) { m_id = value; }
	inline unsigned int getId() const { return m_id; }
	inline void setOutlined(bool value) { m_outlined = value; }
	inline bool geOutlined() const { return m_outlined; }
	inline void setQuaternion(Quaternion* q) { m_quaternion = q; }
	inline Quaternion* getQuaternion() const { return m_quaternion; }

	glm::vec2 getScreenPosition(const SDL::Manager& manager, const Camera& camera) const;

	void draw(const SDL::Manager& manager, const Camera& camera) const;
	void translate(glm::vec3 t);
	void rotate(glm::vec3 pivot, glm::vec3 axis, float angle);

	inline bool operator<=(const Pellet& p) const { return this->m_quaternion->z <= p.m_quaternion->z; }
	inline bool operator>=(const Pellet& p) const { return this->m_quaternion->z >= p.m_quaternion->z; }

	~Pellet();
};
