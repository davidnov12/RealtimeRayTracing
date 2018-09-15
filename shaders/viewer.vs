#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec2 uvs;

uniform mat4 model, view, projection;

out vec3 normal;
out vec3 fragmentPos;
out vec2 tex_coord;

void main(){
	normal = nrm;
	tex_coord = uvs;
	fragmentPos = vec3(vec4(pos, 1.0));

	//gl_Position = model * view * projection * vec4(pos, 1.0);
	//gl_Position = /*projection * view * model */ vec4(0.029 * vec3(pos.x , pos.y-1.24, pos.z-0.3), 1.0);
	
	//gl_Position = /*projection * view * model */ vec4(0.022 * vec3(pos.x , pos.y-0.64, pos.z-0.3), 1.0);
	
	gl_Position = projection * view * model * vec4(pos, 1.0);

}