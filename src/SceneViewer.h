#pragma once
#include "Shader.h"
#include "ModelLoader.h"
#include "CameraCallbacks.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>


class SceneViewer {

public:

	SceneViewer(void* scene, unsigned int vertices, std::vector<glm::vec3> hierarchy, float scale = 1.0f) {
		sceneGeom = scene;
		vertexCount = vertices;
		hierarchyGeom = hierarchy;
		sc = scale;

		init();
	}

	~SceneViewer() {
		glDeleteVertexArrays(1, &sceneVAO);
		glDeleteBuffers(1, &sceneVBO);

		glDeleteVertexArrays(1, &hierVAO);
		glDeleteBuffers(1, &hierVBO);
	}

	void draw() {

		// Clear display
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Matrices settings
		glm::mat4 mod = glm::mat4(), projection, view;
		//mod = glm::translate(mod, glm::vec3(0.0, -0.64, -0.3));
		//mod = glm::rotate(mod, glm::radians(-90.0f), glm::vec3(1,0,0));
		mod = glm::scale(mod, glm::vec3(sc));
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
		view = c.getViewMatrix();

		// Uniforms settings
		modelShader.useProgram();
		glUniformMatrix4fv(glGetUniformLocation(modelShader.getProgram(), "model"), 1, false, &mod[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(modelShader.getProgram(), "view"), 1, false, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(modelShader.getProgram(), "projection"), 1, false, &projection[0][0]);

		// Model draw
		glBindVertexArray(sceneVAO);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		glBindVertexArray(0);

		// Uniforms settings
		hierarchyShader.useProgram();
		glUniformMatrix4fv(glGetUniformLocation(hierarchyShader.getProgram(), "model"), 1, false, &mod[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(hierarchyShader.getProgram(), "view"), 1, false, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(hierarchyShader.getProgram(), "projection"), 1, false, &projection[0][0]);
		
		// BVH draw
		glBindVertexArray(hierVAO);
		glDrawArrays(GL_LINES, 0, hierarchyGeom.size());
		glBindVertexArray(0);

	}

private:

	void init() {

		// Prepare shader for model draw
		modelShader.attachShader(Shader::VERTEX, "shaders/viewer.vs");
		modelShader.attachShader(Shader::FRAGMENT, "shaders/viewer.fs");
		modelShader.linkProgram();

		// Prepare shader for BVH draw
		hierarchyShader.attachShader(Shader::VERTEX, "shaders/bvh_view.vs");
		hierarchyShader.attachShader(Shader::FRAGMENT, "shaders/bvh_view.fs");
		hierarchyShader.linkProgram();

		// Model data - vertices
		glCreateVertexArrays(1, &sceneVAO);
		glCreateBuffers(1, &sceneVBO);
		glBindVertexArray(sceneVAO);
		glBindBuffer(GL_ARRAY_BUFFER, sceneVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertexCount, sceneGeom, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// BVH data
		glCreateVertexArrays(1, &hierVAO);
		glCreateBuffers(1, &hierVBO);
		glBindVertexArray(hierVAO);
		glBindBuffer(GL_ARRAY_BUFFER, hierVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * hierarchyGeom.size(), hierarchyGeom.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glEnable(GL_DEPTH_TEST);
	}

	Shader modelShader, hierarchyShader;
	void* sceneGeom;
	float sc;
	std::vector<glm::vec3> hierarchyGeom;
	unsigned int sceneVAO, sceneVBO, hierVAO, hierVBO, vertexCount;

};
