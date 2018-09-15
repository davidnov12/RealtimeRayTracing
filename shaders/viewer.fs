#version 450

in vec3 normal;
in vec2 tex_coord;
in vec3 fragmentPos;

out vec4 color;

void main(){
	
	vec3 col = vec3(0.3, 0.4, 1);
	vec3 norml = normal;
	vec3 lightPos = vec3(0.0, 80.9, -6.9);
	vec3 lightDir = normalize(lightPos - fragmentPos);
	vec3 ambient = 0.5 * col;
	vec3 diffuse = 0.5 * max(dot(lightDir, norml), 0.01) * col;

	color = vec4(ambient + diffuse, 1.0);

}