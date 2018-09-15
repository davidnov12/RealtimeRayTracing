# RealtimeRayTracing

## Description
Including 3 strategies to BVH building:

- SAH Based BVH - building structure based on SAH (Surface Area Heuristic) metric
- SBVH - based on SAH metric (Object x Spatial division)
- GPU BVH - BVH built on GPU using OpenGL Compute Shaders (based on: https://devblogs.nvidia.com/wp-content/uploads/2012/11/karras2012hpg_paper.pdf)

## Required libraries
- GLEW - http://glew.sourceforge.net/basic.html
- GLFW - http://www.glfw.org/
- Assimp - http://www.assimp.org/
- GLM - https://glm.g-truc.net/0.9.9/index.html
