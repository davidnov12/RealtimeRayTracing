#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>

#define SHADER_CODE 0
#define SHADER_FILE 1
#define DEFAULT_VERBOSE true


class Shader {

public:

	/**
	* \brief contains valid values of shader program types
	*/
	typedef enum {
		VERTEX,
		GEOMETRY,
		TESSELATION_CONTROL,
		TESSELATION_EVALUATION,
		FRAGMENT,
		COMPUTE
	} shaderType;


	/**
	* \brief constructor of the class
	* \param showCompileInfo true = enable printing compilation info (if error occurence) to stdout
	*/
	Shader(bool showCompileInfo);


	/**
	* \brief
	*/
	Shader();


	/**
	*  \brief destructor of the class
	*/
	~Shader();


	/**
	* \brief attach certain shader program to final shader program
	* \param type type of shader program
	* \param sourceCodeOrFile path to file with shader source code or string variable with source code
	* \param mode SHADER_FILE value if param sourceCodeOrFile is path to file, SHADER_CODE if param sourceCodeOrFile is string variable, other values are invalid
	* \return true if all success, otherwise false
	*/
	bool attachShader(shaderType type, std::string sourceCodeOrFile, int mode);


	/**
	* \brief
	* \param type
	* \param sourceCodeOrFile
	* \return
	*/
	bool attachShader(shaderType type, std::string sourceCodeOrFile);


	/**
	* \brief link shader programs to one final shader
	* \return true if link success, otherwise false
	*/
	bool linkProgram();


	/**
	* \brief returns id of shader program
	* \return id of shader program
	*/
	GLuint getProgram();


	/**
	* \brief set this shader program as active
	*/
	void useProgram();

private:

	std::vector<GLuint> shaders, shadersTypes;
	bool verbose;
	GLuint program;


	/**
	* \brief loads content of file
	* \param pathToFile path to file, which will be readed
	* \return content of file in success or empty string
	*/
	std::string loadShaderFile(std::string pathToFile);


	/**
	* \brief
	*/
	void init();

};