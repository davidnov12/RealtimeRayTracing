#pragma once
#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#define TRIANGLE_SET
#define VERTEX_SET

class ModelLoader {

public:

	ModelLoader(std::string path);
	~ModelLoader();

	void processNode(aiNode *node, const aiScene *scene);
	void processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Mesh> getMeshes();
	void* getGeometry();
	std::vector<material> getMaterials();
	unsigned getVertices();
	unsigned getTriangles();
	std::vector<triangle> verticesToTriangles();
	glm::vec3 getMinCoord();
	glm::vec3 getMaxCoord();

private:

	glm::vec3 minCoord, maxCoord;

	std::vector<Mesh> meshes;
#ifdef TRIANGLE_SET
	std::vector<triangle> geom;
#else
#ifdef VERTEX_SET
	std::vector<vertex> geom;
#endif
#endif
	std::vector<material> mats;

};