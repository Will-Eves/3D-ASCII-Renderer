#include "engine.h"
#include <math.h>

#define Vector3 CG::Vector3

int main(void) {
	int w = 60;
	int h = 60;

	CG::setup(w, h);

	CG::RenderBuffer buffer = CG::createBuffer(w, h);
	CG::DepthBuffer dbuffer = CG::createDepthBuffer(w, h);

	CG::RenderBuffer out = CG::createBuffer(w, h);

	CG::mainCamera = CG::Camera(buffer, dbuffer);
	CG::Camera camera = CG::mainCamera;

	CG::MeshRenderer mr2(BasicShapes::Cube, buffer, dbuffer);
	mr2.scale = Vector3(1.5, 1.5, 1.5);

	float time = 0.0f;

	while (true) {
		CG::clear(buffer);
		CG::clear(dbuffer);

		time += 0.006f;

		mr2.rotation = Vector3(time*1.41, time*0.913, time*2.234);

		camera.position.z = cos(time * 5);

		for (int i = 0; i < 6; i++) {
			mr2.position = Vector3(cos(i-time)*2, sin(i-time)*2, sin(i-time)+3.5);
			camera.Render(mr2);
		}

		CG::fillText(buffer, 0, 0, "3D Renderer");

		CG::render(buffer);
	}

	return 0;
}