#pragma once
#include "Mesh.h"
#define SSBO_USAGE
class BoundingVolume {

public:

	BoundingVolume(){}

//protected:

#ifndef SSBO_USAGE

	
	std::vector<triangle> primitives;
	std::unique_ptr<BoundingVolume> left, right, parent;

#else

	
	int start_index, indices_count;
	int left, right, parent;
	glm::vec4 start, end;

#endif

};