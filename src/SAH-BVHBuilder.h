#pragma once
#include "AABB.h"
#include "BVHBuilder.h"

class SAH_BVHBuilder : public BVHBuilder{

public:

	SAH_BVHBuilder(std::vector<triangle> geometry, unsigned int levels = 10, float binLength = 0.2f);

	std::vector<BoundingVolume> buildBVH();
	std::vector<unsigned int> getIndices();

	void bvhConstruction(unsigned int parent, unsigned int node, std::vector<unsigned int> geometry, unsigned levelCount, axis ax = X_AXIS);

	void findMinMax(std::vector<unsigned int> geometry, glm::vec4& minCoord, glm::vec4& maxCoord);
	unsigned int trianglesUnder(std::vector<unsigned int> geometry, float criteria, axis ax);
	std::vector<unsigned int> trianglesLowerBound(std::vector<unsigned int> geometry, float criteria, axis ax);
	std::vector<unsigned int> trianglesUpperBound(std::vector<unsigned int> geometry, float criteria, axis ax);
	
	float evaluateSAH(std::vector<unsigned int> geometry, axis ax);

	std::vector<glm::vec3> bvhToDraw(unsigned int level = 0);


private:

	std::vector<triangle> geom;
	std::vector<BoundingVolume> nodes;
	std::vector<unsigned int> indices;
	float binLen;

	std::vector<glm::vec3> bvhDrawData;
	void traverseToDraw(unsigned int node, unsigned int currentLevel);

};