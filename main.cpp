#include <GLFW/glfw3.h>
#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>

int main(void) {

	if (!glfwInit())
		return 1;

	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

	GLFWwindow *gWindow = glfwCreateWindow(800, 600, "GPUEngine", nullptr, nullptr);
	
	if (gWindow == NULL)
		return 1;
	
	glfwMakeContextCurrent(gWindow);

	ge::gl::init();

	//ge::gl::glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(gWindow)){
		glfwPollEvents();

		//ge::gl::glClearColor(0, 1, 1, 1);
		//ge::gl::glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(gWindow);
	}

	glfwTerminate();

	return 0;
}