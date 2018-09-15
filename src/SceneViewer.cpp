#include "Window.h"
#include "Shader.h"
#include "ModelLoader.h"
#include "CameraCallbacks.h"
#include "SBVHBuilder.h"
#include "SAH-BVHBuilder.h"
#include "GPUBVHBuilder.h"
#include "SceneViewer.h"

#define LEVELS 8

int main(void) {

	Window w(1400, 900, "SceneViewer", true);
	w.setMouseCallback(mouse_callback);

	//ModelLoader ml("model/bunny.obj");	// 291
	//ModelLoader ml("model/US_Galaxy.obj"); // 109.000
	//ModelLoader ml("model/sponza.txt"); // 262.267
	ModelLoader ml("model/dragon.obj"); // 100.000
	
	GPUBVHBuilder sb(ml.verticesToTriangles(), ml.getMinCoord(), ml.getMaxCoord());
	//SBVHBuilder sb(ml.verticesToTriangles(), LEVELS);
	//SAH_BVHBuilder sb(ml.verticesToTriangles(), LEVELS);

	std::vector<glm::vec3> bvh = sb.bvhToDraw(LEVELS);
	
	float sc = (ml.getMaxCoord() - ml.getMinCoord()).x + (ml.getMaxCoord() - ml.getMinCoord()).y + (ml.getMaxCoord() - ml.getMinCoord()).z / 3.0f;
	SceneViewer sv(ml.getGeometry(), ml.getVertices(), bvh, 1.5f / sc);

	while (!w.isClosed()) {

		float current = glfwGetTime();
		delta = current - last;
		last = current;
		key_callback(w.getWindow());
		
		sv.draw();

		w.swapBuffers();
		
	}

	return 0;
}