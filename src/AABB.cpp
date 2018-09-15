#include "AABB.h"

AABB::AABB(glm::vec3 _start, glm::vec3 _end){

#ifndef SSBO_USAGE

	start = _start;
	end = _end;

#else

	start = glm::vec4(_start, 1.0f);
	end = glm::vec4(_end, 1.0f);

#endif

}

AABB::AABB(glm::vec4 _start, glm::vec4 _end){

#ifndef SSBO_USAGE

	start = glm::vec3(_start);
	end = glm::vec3(_end);

#else

	start = _start;
	end = _end;

#endif

}

AABB::~AABB(){

#ifndef SSBO_USAGE

	//primitives.clear();
	//left.reset();

#endif

}

bool AABB::isPointIn(glm::vec3 point){

	if (point.x >= start.x && point.y >= start.y && point.z >= start.z)
		if (point.x <= end.x && point.y <= end.y && point.z <= end.z)
			return true;

	return false;
}
