#pragma once
#include "BoundingVolume.h"
#include <memory>

class BoundingVolumeHierarchy {

public:

	BoundingVolumeHierarchy(std::vector<BoundingVolume> _nodes, std::vector<unsigned int> _indices);
	~BoundingVolumeHierarchy();

	std::vector<BoundingVolume> getStructure();
	std::vector<unsigned int> getIndices();

private:

	std::vector<BoundingVolume> nodes;
	std::vector<unsigned int> indices;

};