#pragma once
#include <glm/glm.hpp>
#include <vector>
#define SSBO_USAGE
#ifndef SSBO_USAGE

typedef struct {
	glm::vec3 vertex_a, vertex_b, vertex_c;
	glm::vec3 normal_a, normal_b, normal_c;
	glm::vec2 uv_a, uv_b, uv_c;
	int material_id;
} triangle;

#else

typedef struct {
	glm::vec4 vertex_a, vertex_b, vertex_c;
	glm::vec4 normal_a, normal_b, normal_c;
	glm::vec2 uv_a, uv_b, uv_c;
	int material_id, gap;
} triangle;

#endif

typedef struct {
	glm::vec3 coord;
	glm::vec3 normal;
	glm::vec2 uv;
	//int material_id;
} vertex;

typedef struct {
	uint64_t diffuse;
	uint64_t normal;
	uint64_t roughness;
	uint64_t metalness;
	uint64_t ambient_occlusion;
} material;

class Mesh {

public:
	
	Mesh(std::vector<triangle> geom = std::vector<triangle>(), std::vector<material> mats = std::vector<material>()) :geometry(geom), materials(mats) {};
	~Mesh();

	std::vector<triangle> getGeometry();
	std::vector<material> getMaterials();

private:

	std::vector<triangle> geometry;
	std::vector<material> materials;

};