#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window {

public:

	/**
	* \brief constructor of the class
	* \param width width of window (in pixels)
	* \param height height of window (in pixels)
	* \param title title of window
	* \param createImmediately if true window will be created immediately, else window must be created manually
	*/
	Window(int width, int height, std::string title, bool createImmediately);


	/**
	* \brief destructor of the class
	*/
	~Window();


	/**
	* \brief gets current window close state
	* \return true if window gets close, else false
	*/
	bool isClosed();


	/**
	* \brief swaps framebuffers
	*/
	void swapBuffers();


	/**
	* \brief function to get window object
	* \return reference to window object
	*/
	GLFWwindow* getWindow();


	/**
	* \brief function to get window width
	* \return window width in pixels
	*/
	int getWidth();


	/**
	* \brief function to get window height
	* \return window height in pixels
	*/
	int getHeight();


	/**
	* \brief make this context current
	*/
	void makeContextCurrent();


	/**
	* \brief sets some window property
	* \param property type of window property
	* \param value value of setting property
	*/
	void setWindowProperty(int property, int value);


	/**
	* \brief sets window properties to default values
	*/
	void setDefaults();


	/**
	* \brief creates graphics context and window
	* \return true if success, else false
	*/
	bool createWindow();


	/**
	* \brief allow to enable some feature (depth_test, blend etc.)
	* \param feature type of enabling feature
	*/
	void enableFeature(int feature);


	/**
	* \brief
	* \param function
	*/
	void setKeyCallback(GLFWkeyfun function);


	/**
	* \brief
	* \param function
	*/
	void setMouseCallback(GLFWcursorposfun function);


	/**
	* \brief
	* \param function
	*/
	void setCursorCallback(GLFWcursorposfun function);


	/**
	* \brief
	* \param function
	*/
	void setResizeCallback(GLFWwindowsizefun function);


	/**
	* \brief
	* \param function
	*/
	void setWindowRefreshCallback(GLFWwindowrefreshfun function);


	/**
	* \brief
	* \param function
	*/
	void setScrollCallback(GLFWscrollfun function);

private:

	struct DestroyWin {

		void operator()(GLFWwindow* ptr) {
			glfwDestroyWindow(ptr);
		}

	};


	int width, height;
	std::string title;
	std::unique_ptr<GLFWwindow, DestroyWin> window;

};