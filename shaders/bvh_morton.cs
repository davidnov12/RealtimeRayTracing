#version 450

layout(local_size_x = 1, local_size_y = 1) in;

struct triangle{
	vec3 position_a;
	vec3 position_b;
	vec3 position_c;
	vec3 normal_a;
	vec3 normal_b;
	vec3 normal_c;
	vec2 uv_a;
	vec2 uv_b;
	vec2 uv_c;
	int material_id;
};


uniform vec3 minCoord, maxCoord;

layout(std430, binding = 0) readonly buffer inBuffer{
    triangle inTriangles[];
};

layout(std430, binding = 1) writeonly buffer outBuffer{
    uint outMortons[];
};

vec3 computeCentroid(){
	triangle tg = inTriangles[gl_GlobalInvocationID.x];
	return (tg.position_a + tg.position_b + tg.position_c) / 3.0f;
}

vec3 normalizeCentroid(vec3 center){
	vec3 new_pos = center - minCoord;
	vec3 len = maxCoord - minCoord;
	new_pos /= len;
	return new_pos;
}

uint expandBits(uint x){
	x = (x * 0x00010001u) & 0xFF0000FFu;
  x = (x * 0x00000101u) & 0x0F00F00Fu;
  x = (x * 0x00000011u) & 0xC30C30C3u;
  x = (x * 0x00000005u) & 0x49249249u;
  return x;
}

uint mortonCode(vec3 coord){

	coord.x = min(max(coord.x * 1024.0f, 0.0f), 1023.0f);
	coord.y = min(max(coord.y * 1024.0f, 0.0f), 1023.0f);
	coord.z = min(max(coord.z * 1024.0f, 0.0f), 1023.0f);

	uint x = expandBits(uint(coord.x));
	uint y = expandBits(uint(coord.y));
	uint z = expandBits(uint(coord.z));

	return x * 4 + y * 2 + z;
}


void main(){

	vec3 cent = computeCentroid();
	cent = normalizeCentroid(cent);
	uint res = mortonCode(cent);

	outMortons[gl_GlobalInvocationID.x] = res;
	//outMortons[gl_GlobalInvocationID.x] = 22;

}
