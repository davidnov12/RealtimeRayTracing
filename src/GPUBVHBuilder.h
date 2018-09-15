#pragma once
#include "BVHBuilder.h"
#include "Shader.h"
#include <GL/glew.h>
#include <stack>

typedef struct {
	glm::vec4 start, end;
	glm::ivec4 lr;//int left, right, t1, t2;
} gpu_node;

class GPUBVHBuilder {

public:

	GPUBVHBuilder(std::vector<triangle> geometry, glm::vec3 _min = glm::vec3(0.0f), glm::vec3 _max = glm::vec3(1.0f));
	~GPUBVHBuilder();

	std::vector<gpu_node> buildBVH();
	
	void traverse(int node = 0);
	unsigned int getBVHHeight();
	std::vector<glm::vec3> bvhToDraw(unsigned int n);

private:

	void initGLObjects(std::vector<triangle> geometry);
	void createBVH(unsigned int triangles);
	
	unsigned int maxHeight = 0;
	std::vector<gpu_node> nodes;
	std::vector<glm::vec3> bvhDrawData;
	glm::vec3 minCoord, maxCoord;
	unsigned int geomSSBO, histSSBO, mortonSSBO[2], indexSSBO[2], nodesSSBO;
	char active = 0;
	Shader mortonShader, radixSortShader, radixTreeShader;
};