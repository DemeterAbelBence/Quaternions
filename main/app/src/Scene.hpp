#include <iostream>
#include <vector>

#include "SDL2/SDL.hpp"
#include "Pellet.hpp"
#include "Collection.hpp"
#include "Camera.hpp"

class Scene {
private:
	unsigned int m_pellet_id;
	unsigned int m_axis_pellet_a;
	unsigned int m_axis_pellet_b;

	float m_move_speed;
	float m_rotate_speed;

private:
	SDL::Event* m_event;
	Collection<Pellet>* m_pellets;
	Camera* m_camera;

private:
	Pellet* getPelletById(unsigned int id) const;
	void connectAxisPellets(const SDL::Manager& manager) const;
	void addNewPellet(Quaternion* q, SDL::Color* c);
	void generatePellets();

public:
	Scene();

	inline void setSelectedAsAxisA() { m_axis_pellet_a = m_pellet_id; }
	inline void setSelectedAsAxisB() { m_axis_pellet_b = m_pellet_id; }
	inline SDL::Event* getEvent() const { return m_event; }
	inline Camera* getCamera() const { return m_camera; }

	void incrementPelletId();
	bool update(const SDL::Manager& manager);
	void draw(const SDL::Manager& manager) const;

	~Scene();
};