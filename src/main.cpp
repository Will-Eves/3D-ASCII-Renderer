#include "engine.h"
#include <math.h>

#define PI 3.14159

int main(void) {
	int w = 280;
	int h = 280;

	// use these characters next!
	// uses 1/4 the space of the original characters, which should
	// help with the printing bottleneck as well as making the
	// screen smaller
	// std::cout << "▀" << std::endl;

	CG::setup(w, h);
	CG::clearScreen();

	CG::RenderBuffer buffer = CG::createBuffer(w, h);
	CG::DepthBuffer dbuffer = CG::createDepthBuffer(w, h);

	CG::mainCamera = CG::Camera(buffer, dbuffer);
	CG::Camera camera = CG::mainCamera;
	camera.position.z = -5.0f;

	CG::MeshRenderer mr2(BasicShapes::Cube, buffer, dbuffer);
	mr2.scale = Vector3(1.5, 1.5, 1.5);

	float time = 0.0f;

	int cubeCount = 2500;
	float ratio = PI * 2.0f / cubeCount;

	FPS fps;

	while (true) {
		fps.startUpdate();

		CG::clear(buffer, CG::Color(90, 110, 220));
		CG::clear(dbuffer);

		time += 1.0f * fps.deltaTime;

		srand(68);
		for(float f = 0; f < PI * 2; f += ratio){
			mr2.position = Vector3(rand() % 75 - 37, rand() % 75 - 37, rand() % 50);
			float rotDiff = (rand() % 10000)/100.0f;
			mr2.rotation = Vector3((time + rotDiff)*1.41, (time + rotDiff)*0.913, (time + rotDiff)*2.234);
			camera.Render(mr2);
		}

		// CG::fillText(buffer, 0, 0, "3D Renderer");
		// CG::fillText(buffer, 0, 1, "FPS : " + std::to_string(fps.fps));

		CG::render(buffer);

		fps.endUpdate();
	}

	CG::end();

	return 0;
}
