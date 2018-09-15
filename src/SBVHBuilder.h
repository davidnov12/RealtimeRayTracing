#pragma once
#include <chrono>
#include <iostream>
#include "BVHBuilder.h"
#include "AABB.h"

#define BVH_DRAW

class SBVHBuilder : public BVHBuilder {

public:

	SBVHBuilder(std::vector<triangle> geometry, unsigned int levels = 10, float binLength = 0.2f);

	std::vector<BoundingVolume> buildBVH();
	std::vector<unsigned int> getIndices();
	
	void bvhConstruction(unsigned int parent, unsigned int node, std::vector<unsigned int> geometry, unsigned levelCount, glm::vec4 lBorder, glm::vec4 rBorder, spliting spl, axis ax = X_AXIS);
	void findMinMax(std::vector<unsigned int> geometry, glm::vec4& minCoord, glm::vec4& maxCoord);
	
	unsigned int trianglesUnder(std::vector<unsigned int> geometry, float criteria, axis ax);
	float trianglesUnderSpatial(std::vector<unsigned int> geometry, float criteria, float _min, axis ax);
	std::vector<unsigned int> trianglesLowerBound(std::vector<unsigned int> geometry, float criteria, axis ax, spliting split = OBJECT);
	std::vector<unsigned int> trianglesUpperBound(std::vector<unsigned int> geometry, float criteria, axis ax, spliting split = OBJECT);
	

	float evaluateSAH(std::vector<unsigned int> geometry, axis ax, spliting& split);
	float evaluateObjectSAH(std::vector<unsigned int> geometry, axis ax, float& sahValue);
	float evaluateSpatialSAH(std::vector<unsigned int> geometry, axis ax, float& sahValue);

//#ifdef BVH_DRAW
	std::vector<glm::vec3> bvhToDraw(unsigned int level = 0);
//#endif

private:

	std::vector<triangle> geom;
	std::vector<BoundingVolume> nodes;
	std::vector<unsigned int> indices;
	float binLen;

	std::vector<glm::vec4> centroids;
	void computeCenters();

//#ifdef BVH_DRAW
	std::vector<glm::vec3> bvhDrawData;
	void traverseToDraw(unsigned int node, unsigned int currentLevel);
//#endif

};