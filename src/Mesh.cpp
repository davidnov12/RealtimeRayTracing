#include "Mesh.h"

Mesh::~Mesh(){
	geometry.clear();
	materials.clear();
}

std::vector<triangle> Mesh::getGeometry(){
	return geometry;
}

std::vector<material> Mesh::getMaterials(){
	return materials;
}
