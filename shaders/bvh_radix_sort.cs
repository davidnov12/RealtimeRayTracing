#version 450

#define HISTOGRAM 0
#define REORDER 1

layout(local_size_x = 1, local_size_y = 1) in;


layout(std430, binding = 0) buffer inBuffer{
    uint inMortons[];
};

layout(std430, binding = 1) buffer histBuffer{
    uint mask[];
};

layout(std430, binding = 2) buffer inMBuffer{
    uint mortonIndices[];
};

layout(std430, binding = 3) buffer outMBuffer{
    uint mortonOutIndices[];
};

layout(std430, binding = 4) buffer outBuffer{
    uint outMortons[];
};

uniform uint mode;
uniform uint base;
uniform uint count;

void computeHistogram(){
  if((inMortons[gl_GlobalInvocationID.x] & base) == base)
    atomicAdd(mask[1], 1);
  else
    atomicAdd(mask[0], 1);
}

void computePrefixMask(){
  mask[1] = mask[0];
  mask[0] = 0;

}

void reorderElements(){

  for(uint i = 0; i < count; i++){
    if((inMortons[i] & base) == base){
      outMortons[mask[1]] = inMortons[i];
      mortonOutIndices[mask[1]] = mortonIndices[i];
      mask[1]++;
    }
    else{
      outMortons[mask[0]] = inMortons[i];
      mortonOutIndices[mask[0]] = mortonIndices[i];
      mask[0]++;
    }

  }
  mask[0] = 0;
  mask[1] = 0;

}

void main(){
  if(mode == HISTOGRAM)
    computeHistogram();
  else if(mode == REORDER){
    computePrefixMask();
    reorderElements();
  }
  //else if(mode == REORDER)
  //  reorderElements();
}
