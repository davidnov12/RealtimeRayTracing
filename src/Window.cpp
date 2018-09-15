#include "Window.h"


Window::Window(int width, int height, std::string title, bool createImmediately) {
	this->width = width;
	this->height = height;
	this->title = title;

	if (createImmediately)
		setDefaults(), createWindow();

}


Window::~Window() {
	glfwDestroyWindow(window.get());
	glfwTerminate();
	window.release();
}


bool Window::isClosed() {
	if (glfwWindowShouldClose(window.get()))
		return true;

	glfwPollEvents();
	return false;
}


void Window::swapBuffers() {
	glfwSwapBuffers(window.get());
}


GLFWwindow* Window::getWindow() {
	return window.get();
}


int Window::getWidth() {
	return width;
}


int Window::getHeight() {
	return height;
}


void Window::makeContextCurrent() {
	glfwMakeContextCurrent(window.get());
}


void Window::setWindowProperty(int property, int value) {
	glfwWindowHint(property, value);
}


void Window::setDefaults() {
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 16);
}


bool Window::createWindow() {

	if (!glfwInit())
		return false;

	window.reset(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));

	if (window.get() == nullptr)
		return false;

	glfwMakeContextCurrent(window.get());

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return false;
	}

	// VSYNC OFF
	//glfwSwapInterval(0);

	int w, h;
	glfwGetFramebufferSize(window.get(), &w, &h);
	glViewport(0, 0, w, h);

	return true;
}


void Window::enableFeature(int feature) {
	glEnable(feature);
}


void Window::setKeyCallback(GLFWkeyfun function) {
	glfwSetKeyCallback(window.get(), function);
}


void Window::setMouseCallback(GLFWcursorposfun function) {
	glfwSetCursorPosCallback(window.get(), function);
}


void Window::setCursorCallback(GLFWcursorposfun function) {
	glfwSetCursorPosCallback(window.get(), function);
}


void Window::setResizeCallback(GLFWwindowsizefun function) {
	glfwSetWindowSizeCallback(window.get(), function);
}


void Window::setWindowRefreshCallback(GLFWwindowrefreshfun function) {
	glfwSetWindowRefreshCallback(window.get(), function);
}


void Window::setScrollCallback(GLFWscrollfun function) {
	glfwSetScrollCallback(window.get(), function);
}