#include "GPUBVHBuilder.h"

GPUBVHBuilder::GPUBVHBuilder(std::vector<triangle> geometry, glm::vec3 _min, glm::vec3 _max){

	nodes = std::vector<gpu_node>(geometry.size() - 1);;
	minCoord = _min;
	maxCoord = _max;

	initGLObjects(geometry);
	createBVH(geometry.size());

}

GPUBVHBuilder::~GPUBVHBuilder(){

	glDeleteBuffers(1, &geomSSBO);
	glDeleteBuffers(1, &nodesSSBO);
	glDeleteBuffers(1, &histSSBO);
	glDeleteBuffers(2, indexSSBO);
	glDeleteBuffers(2, mortonSSBO);
	bvhDrawData.shrink_to_fit();
	nodes.shrink_to_fit();

}

std::vector<gpu_node> GPUBVHBuilder::buildBVH(){
	return nodes;
}

void GPUBVHBuilder::traverse(int node){

	std::stack<int> rec_stack;
	int current = node;
	unsigned int height = 0;
	rec_stack.push(node);

	while (current != -1 || !rec_stack.empty()) {
		while (current != -1) {
			//cnt++;
			//printf("%d %d\n", current, cnt);
			rec_stack.push(current);
			current = nodes[current].lr.x;
			height += 1;
			maxHeight = std::max(maxHeight, height);
		}
		current = rec_stack.top();
		rec_stack.pop();
		height -= 1;
		current = nodes[current].lr.y;
	}

	//maxHeight = std::max(maxHeight, height);

	//if (nodes[node].lr.x != -1) traverse(nodes[node].lr.x, height + 1);
	//if (nodes[node].lr.y != -1) traverse(nodes[node].lr.y, height + 1);

}

unsigned int GPUBVHBuilder::getBVHHeight(){

	return maxHeight;
}

std::vector<glm::vec3> GPUBVHBuilder::bvhToDraw(unsigned int n){

	for (unsigned int i = 0; i < nodes.size(); i++) {

		if (nodes[i].lr.z != -1 || nodes[i].lr.w != -1) {

			bvhDrawData.push_back(glm::vec3(nodes[i].start));
			bvhDrawData.push_back(glm::vec3(nodes[i].start.x, nodes[i].end.y, nodes[i].start.z));

			bvhDrawData.push_back(glm::vec3(nodes[i].start));
			bvhDrawData.push_back(glm::vec3(nodes[i].end.x, nodes[i].start.y, nodes[i].start.z));

			bvhDrawData.push_back(glm::vec3(nodes[i].start.x, nodes[i].end.y, nodes[i].start.z));
			bvhDrawData.push_back(glm::vec3(nodes[i].end.x, nodes[i].end.y, nodes[i].start.z));

			bvhDrawData.push_back(glm::vec3(nodes[i].end.x, nodes[i].start.y, nodes[i].start.z));
			bvhDrawData.push_back(glm::vec3(nodes[i].end.x, nodes[i].end.y, nodes[i].start.z));

			bvhDrawData.push_back(glm::vec3(nodes[i].start));
			bvhDrawData.push_back(glm::vec3(nodes[i].start.x, nodes[i].start.y, nodes[i].end.z));

			bvhDrawData.push_back(glm::vec3(nodes[i].end.x, nodes[i].end.y, nodes[i].start.z));
			bvhDrawData.push_back(glm::vec3(nodes[i].end));

			bvhDrawData.push_back(glm::vec3(nodes[i].start.x, nodes[i].end.y, nodes[i].start.z));
			bvhDrawData.push_back(glm::vec3(nodes[i].start.x, nodes[i].end.y, nodes[i].end.z));

			bvhDrawData.push_back(glm::vec3(nodes[i].end.x, nodes[i].start.y, nodes[i].start.z));
			bvhDrawData.push_back(glm::vec3(nodes[i].end.x, nodes[i].start.y, nodes[i].end.z));

			bvhDrawData.push_back(glm::vec3(nodes[i].start.x, nodes[i].start.y, nodes[i].end.z));
			bvhDrawData.push_back(glm::vec3(nodes[i].start.x, nodes[i].end.y, nodes[i].end.z));

			bvhDrawData.push_back(glm::vec3(nodes[i].start.x, nodes[i].start.y, nodes[i].end.z));
			bvhDrawData.push_back(glm::vec3(nodes[i].end.x, nodes[i].start.y, nodes[i].end.z));

			bvhDrawData.push_back(glm::vec3(nodes[i].start.x, nodes[i].end.y, nodes[i].end.z));
			bvhDrawData.push_back(glm::vec3(nodes[i].end));

			bvhDrawData.push_back(glm::vec3(nodes[i].end.x, nodes[i].start.y, nodes[i].end.z));
			bvhDrawData.push_back(glm::vec3(nodes[i].end));

		}
	}
	
	return bvhDrawData;
}

void GPUBVHBuilder::initGLObjects(std::vector<triangle> geometry){

	std::vector<unsigned int> indices(geometry.size());

	for (unsigned int i = 0; i < indices.size(); i++)
		indices[i] = i;

	// geometry SSBO
	glGenBuffers(1, &geomSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, geomSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(triangle) * geometry.size(), geometry.data(), GL_STATIC_DRAW);

	// histogram SSBO - radix sort
	glGenBuffers(1, &histSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, histSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int) * 2, NULL, GL_STATIC_DRAW);

	// nodes SSBO
	glGenBuffers(1, &nodesSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodesSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(gpu_node) * (geometry.size() - 1), NULL, GL_STATIC_DRAW);

	// morton codes SSBO
	glGenBuffers(2, mortonSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mortonSSBO[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int) * geometry.size(), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mortonSSBO[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int) * geometry.size(), NULL, GL_STATIC_DRAW);

	// triangles indices SSBO
	glGenBuffers(2, indexSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, indexSSBO[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int) * geometry.size(), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, indexSSBO[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(unsigned int) * geometry.size(), NULL, GL_STATIC_DRAW);


	mortonShader.attachShader(Shader::COMPUTE, "shaders/bvh_morton.cs");
	mortonShader.linkProgram();

	radixSortShader.attachShader(Shader::COMPUTE, "shaders/bvh_radix_sort.cs");
	radixSortShader.linkProgram();

	radixTreeShader.attachShader(Shader::COMPUTE, "shaders/bvh_radix_tree.cs");
	radixTreeShader.linkProgram();
}

void GPUBVHBuilder::createBVH(unsigned int triangles){

#define HISTOGRAM 0
#define REORDER 1

	// Normalize to unit cube and compute morton codes 
	mortonShader.useProgram();
	glUniform3f(glGetUniformLocation(mortonShader.getProgram(), "minCoord"), minCoord.x, minCoord.y, minCoord.z);
	glUniform3f(glGetUniformLocation(mortonShader.getProgram(), "maxCoord"), maxCoord.x, maxCoord.y, maxCoord.z);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, geomSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mortonSSBO[active]);
	
	glDispatchCompute(triangles, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	

	// Parallel binary radix sort (30 bits)
	unsigned int base = 1;
	radixSortShader.useProgram();
	for (unsigned i = 0; i < 30; i++) {
		
		glUniform1ui(glGetUniformLocation(radixSortShader.getProgram(), "base"), base << i);
		glUniform1ui(glGetUniformLocation(radixSortShader.getProgram(), "mode"), HISTOGRAM);
		glUniform1ui(glGetUniformLocation(radixSortShader.getProgram(), "count"), triangles);
		
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mortonSSBO[active]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, histSSBO);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, indexSSBO[active]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, indexSSBO[1 - active]);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, mortonSSBO[1 - active]);
		
		active++;
		if (active == 2) active = 0;
		
		glDispatchCompute(triangles, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		glUniform1ui(glGetUniformLocation(radixSortShader.getProgram(), "mode"), REORDER);
		
		glDispatchCompute(1, 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	}

	/*std::vector<unsigned> mt(triangles);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mortonSSBO[active]);
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	memcpy(mt.data(), p, sizeof(unsigned) * (triangles));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	for (unsigned i = 0; i < mt.size(); i++)
		printf("%d %d\n", i, mt[i]);*/

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, 0);

	

	// Build radix tree
	radixTreeShader.useProgram();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mortonSSBO[active]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, indexSSBO[active]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, geomSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, nodesSSBO);

	glDispatchCompute(triangles - 1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, 0);


	
	// Get nodes data from GPU
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, nodesSSBO);
	GLvoid* q = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
	memcpy(nodes.data(), q, sizeof(gpu_node) * (triangles - 1));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	printf("nodes %d\n", nodes.size());
		
}
