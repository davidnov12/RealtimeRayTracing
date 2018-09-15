#pragma once
#include <memory>
#include <algorithm>
#include <limits>
#include "BoundingVolume.h"

typedef enum {
	X_AXIS,
	Y_AXIS,
	Z_AXIS
} axis;

typedef enum {
	OBJECT,
	SPATIAL
} spliting;

class BVHBuilder {
	
public:

	BVHBuilder(){}
	
	virtual std::vector<BoundingVolume> buildBVH() { return std::vector<BoundingVolume>(); }
	virtual std::vector<unsigned int> getIndices() { return std::vector<unsigned int>(); }
	virtual std::vector<glm::vec3> bvhToDraw(unsigned int level = 0) { return std::vector<glm::vec3>(); }
};