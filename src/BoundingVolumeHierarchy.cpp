#include "BoundingVolumeHierarchy.h"


BoundingVolumeHierarchy::BoundingVolumeHierarchy(std::vector<BoundingVolume> _nodes, std::vector<unsigned int> _indices){
	nodes = _nodes;
	indices = _indices;
}

BoundingVolumeHierarchy::~BoundingVolumeHierarchy() {
	nodes.clear();
	indices.clear();
}

std::vector<BoundingVolume> BoundingVolumeHierarchy::getStructure(){
	return nodes;
}

std::vector<unsigned int> BoundingVolumeHierarchy::getIndices(){
	return indices;
}
