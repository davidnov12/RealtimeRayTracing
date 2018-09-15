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

struct node{
  vec3 start;
  vec3 end;
	ivec4 lr;
  /*int left;
  int right;
  int t1, t2;*/
};

layout(std430, binding = 0) buffer inBuffer{
    uint inMortons[];
};

layout(std430, binding = 1) buffer indexBuffer{
    uint indices[];
};

layout(std430, binding = 2) buffer geomBuffer{
    triangle geometry[];
};

layout(std430, binding = 3) buffer nodeBuffer{
    node nodes[];
};

int sgn(int x){
  if(x > 0) return 1;
  else if(x < 0) return -1;
  return 0;
}

int delta(uint a, uint b){
  if(a < 0 || b > gl_NumWorkGroups.x)
    return -1;

  uint res = inMortons[a] ^ inMortons[b];
  return int(31 - log2(float(res)));
}

void findMinMax(int i, int start, int end){

  triangle m, n;
  m = geometry[indices[start]];
  n = geometry[indices[end]];

  nodes[i].start.x = min(min(m.position_a.x, m.position_b.x), m.position_c.x);
  nodes[i].start.y = min(min(m.position_a.y, m.position_b.y), m.position_c.y);
  nodes[i].start.z = min(min(m.position_a.z, m.position_b.z), m.position_c.z);

  nodes[i].end.x = max(max(n.position_a.x, n.position_b.x), n.position_c.x);
  nodes[i].end.y = max(max(n.position_a.y, n.position_b.y), n.position_c.y);
  nodes[i].end.z = max(max(n.position_a.z, n.position_b.z), n.position_c.z);

}

void main(){

  int i = int(gl_GlobalInvocationID.x);
	nodes[i].lr = ivec4(-1);
  int d = sgn(delta(i, i + 1) - delta(i, i - 1));

  int d_min = delta(i, i - d);
  int l_max = 2;

  while(delta(i, i + (l_max * d)) > d_min)
    l_max *= 2;

  int l = 0;
  for(int t = l_max / 2; t >= 1; t /= 2){
    if(delta(i, i + ((l + t) * d)) > d_min)
      l += t;
  }

  int j = i + l * d;
  int d_node = delta(i, j);

  int s = 0;
  for(int t = l / 2; t >= 1; t /= 2){
    if(delta(i, i + (s + t) * d) > d_node)
      s += t;
  }

  int y = i + s * d + min(d, 0);
  //int left, right;

  if(min(i, j) == y){
		//nodes[i].t1 = y;
		nodes[i].lr.z = y;
		//nodes[i].left = 0;
	}
    //left = ;
  else{
		//nodes[i].t1 = 0;
		nodes[i].lr.x = y;
		//nodes[i].left = y;
	}
    //left = ;

  if(max(i, j) == (y + 1)){
		//nodes[i].t2 = y + 1;
		nodes[i].lr.w = y + 1;
		//nodes[i].right = 0;
	}
  //  right = ;
  else{
		//nodes[i].t2 = 0;
		nodes[i].lr.y = y + 1;
		//nodes[i].right = y + 1;
	}
  //  right = ;

  findMinMax(i, min(i, j), max(i, j));

}
