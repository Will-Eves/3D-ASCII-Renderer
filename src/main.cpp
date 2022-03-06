#include "engine.h"
#include "fps.h"
#include "input.h"
#include <math.h>

#define PI 3.14159

int main(void) {
	int w = 60;
	int h = 60;

	CG::setup(w, h);

	CG::RenderBuffer buffer = CG::createBuffer(w, h);
	CG::DepthBuffer dbuffer = CG::createDepthBuffer(w, h);

	CG::mainCamera = CG::Camera(buffer, dbuffer);
	CG::Camera camera = CG::mainCamera;
	camera.position.z = -5.0f;

	CG::Mesh mesh("res/Models/Monkey.obj", CG::getColor(255));

	CG::MeshRenderer mr2(BasicShapes::Cube, buffer, dbuffer);
	mr2.scale = Vector3(1.5, 1.5, 1.5);

	float time = 0.0f;

	int cubeCount = 1000;
	float ratio = PI * 2.0f / cubeCount;

	FPS fps;

	while (true) {
		fps.startUpdate();

		CG::clear(buffer);
		CG::clear(dbuffer);

		time += 1.0f * fps.deltaTime;

		mr2.rotation = Vector3(time*1.41, time*0.913, time*2.234);

		for(float f = 0; f < PI * 2; f += ratio){
			mr2.position = Vector3(cos(f) * 3.0f, sin(f) * 3.0f, 0.0f);
			camera.Render(mr2);
		}

		CG::fillText(buffer, 0, 0, "3D Renderer");
		CG::fillText(buffer, 0, 1, "FPS : " + std::to_string(fps.fps));

		CG::render(buffer);

		fps.endUpdate();
	}

	return 0;
}