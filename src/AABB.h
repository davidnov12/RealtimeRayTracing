#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "BoundingVolume.h"
#include "Mesh.h"
#define SSBO_USAGE
class AABB : public BoundingVolume {

public:

	AABB(glm::vec3 _start, glm::vec3 _end);
	AABB(glm::vec4 _start, glm::vec4 _end);
	~AABB();

	bool isPointIn(glm::vec3 point);

//private:

#ifndef SSBO_USAGE

	glm::vec3 start, end;

#else

	//glm::vec4 start, end;

#endif

};