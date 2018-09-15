cmake_minimum_required (VERSION 3.3)

project(RayTracingGPUE)

find_package(OpenGL REQUIRED)

set(ASSIMP_INCLUDE_PATH "" CACHE PATH "assimp include directory" )
set(ASSIMP_LIBRARY "" CACHE FILEPATH "libassimp.so or assimp.lib" )
set(ASSIMP_DYNAMIC_LIBRARY "" CACHE FILEPATH "libassimp.a or assimp.dll" )

set(GLFW_INCLUDE_PATH "" CACHE PATH "glfw include directory" )
set(GLFW_LIBRARY "" CACHE FILEPATH "libglfw.so or glfw.lib" )
set(GLFW_DYNAMIC_LIBRARY "" CACHE FILEPATH "libglfw.a or glfw.dll" )

set(GLEW_INCLUDE_PATH "" CACHE PATH "glew include directory" )
set(GLEW_LIBRARY "" CACHE FILEPATH "libglew.so or glew.lib" )
set(GLEW_DYNAMIC_LIBRARY "" CACHE FILEPATH "libglew.a or glew.dll" )

set(GLM_INCLUDE_PATH "" CACHE PATH "glm directory" )


set(HEADER_FILES
	src/AABB.h
	src/BoundingVolume.h
	src/BoundingVolumeHierarchy.h
	src/BVHBuilder.h
	src/Camera.h
	src/CameraCallbacks.h
	src/FPScamera.h
	src/GPUBVHBuilder.h
	src/Mesh.h
	src/ModelLoader.h
	src/SAH-BVHBuilder.h
	src/SBVHBuilder.h
	src/SceneViewer.h
	src/Shader.h
	src/Window.h
)

set(SOURCE_FILES
	src/AABB.cpp
	src/BoundingVolumeHierarchy.cpp
	src/FPScamera.cpp
	src/GPUBVHBuilder.cpp
	src/Mesh.cpp
	src/ModelLoader.cpp
	src/SAH-BVHBuilder.cpp
	src/SBVHBuilder.cpp
	src/SceneViewer.cpp
	src/Shader.cpp
	src/Window.cpp
)

string(FIND ${ASSIMP_DYNAMIC_LIBRARY} "/" pozice REVERSE)
string(LENGTH ${ASSIMP_DYNAMIC_LIBRARY} delka)
string(CONCAT exp ${delka} "-" ${pozice} "-1")
math(EXPR pos ${exp})
string(CONCAT pozice ${pozice} "+1")
math(EXPR pozice ${pozice})
string(SUBSTRING ${ASSIMP_DYNAMIC_LIBRARY} ${pozice} ${pos} filename)
set(ASSIMP_DYN ${CMAKE_CURRENT_BINARY_DIR}/${filename})

string(FIND ${GLFW_DYNAMIC_LIBRARY} "/" pozice REVERSE)
string(LENGTH ${GLFW_DYNAMIC_LIBRARY} delka)
string(CONCAT exp ${delka} "-" ${pozice} "-1")
math(EXPR pos ${exp})
string(CONCAT pozice ${pozice} "+1")
math(EXPR pozice ${pozice})
string(SUBSTRING ${GLFW_DYNAMIC_LIBRARY} ${pozice} ${pos} filename)
set(GLFW_DYN ${CMAKE_CURRENT_BINARY_DIR}/${filename})

string(FIND ${GLEW_DYNAMIC_LIBRARY} "/" pozice REVERSE)
string(LENGTH ${GLEW_DYNAMIC_LIBRARY} delka)
string(CONCAT exp ${delka} "-" ${pozice} "-1")
math(EXPR pos ${exp})
string(CONCAT pozice ${pozice} "+1")
math(EXPR pozice ${pozice})
string(SUBSTRING ${GLEW_DYNAMIC_LIBRARY} ${pozice} ${pos} filename)
set(GLEW_DYN ${CMAKE_CURRENT_BINARY_DIR}/${filename})

configure_file(${GLEW_DYNAMIC_LIBRARY} ${GLEW_DYN} COPYONLY)
configure_file(${GLFW_DYNAMIC_LIBRARY} ${GLFW_DYN} COPYONLY)
configure_file(${ASSIMP_DYNAMIC_LIBRARY} ${ASSIMP_DYN} COPYONLY)

# models
set(BUNNY_MODEL_IN ${CMAKE_CURRENT_SOURCE_DIR}/model/bunny.obj)
set(DRAGON_MODEL_IN ${CMAKE_CURRENT_SOURCE_DIR}/model/dragon.obj)
set(GALAXY_MODEL_IN ${CMAKE_CURRENT_SOURCE_DIR}/model/galaxy.obj)
set(SPONZA_MODEL_IN ${CMAKE_CURRENT_SOURCE_DIR}/model/sponza.obj)
set(BUNNY_MODEL_OUT ${CMAKE_CURRENT_BINARY_DIR}/model/bunny.obj)
set(DRAGON_MODEL_OUT ${CMAKE_CURRENT_BINARY_DIR}/model/dragon.obj)
set(GALAXY_MODEL_OUT ${CMAKE_CURRENT_BINARY_DIR}/model/galaxy.obj)
set(SPONZA_MODEL_OUT ${CMAKE_CURRENT_BINARY_DIR}/model/sponza.obj)
configure_file(${BUNNY_MODEL_IN} ${BUNNY_MODEL_OUT} COPYONLY)
configure_file(${DRAGON_MODEL_IN} ${DRAGON_MODEL_OUT} COPYONLY)
configure_file(${GALAXY_MODEL_IN} ${GALAXY_MODEL_OUT} COPYONLY)
configure_file(${SPONZA_MODEL_IN} ${SPONZA_MODEL_OUT} COPYONLY)

# shaders
set(VIEW_SHADER_VS_IN ${CMAKE_CURRENT_SOURCE_DIR}/shaders/viewer.vs)
set(VIEW_SHADER_FS_IN ${CMAKE_CURRENT_SOURCE_DIR}/shaders/viewer.fs)
set(BVH_SHADER_VS_IN ${CMAKE_CURRENT_SOURCE_DIR}/shaders/bvh_view.vs)
set(BVH_SHADER_FS_IN ${CMAKE_CURRENT_SOURCE_DIR}/shaders/bvh_view.fs)
set(MORTON_SHADER_CS_IN ${CMAKE_CURRENT_SOURCE_DIR}/shaders/bvh_morton.cs)
set(RADIXSORT_SHADER_CS_IN ${CMAKE_CURRENT_SOURCE_DIR}/shaders/bvh_radix_sort.cs)
set(RADIXTREE_SHADER_CS_IN ${CMAKE_CURRENT_SOURCE_DIR}/shaders/bvh_radix_tree.cs)
set(VIEW_SHADER_VS_OUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/viewer.vs)
set(VIEW_SHADER_FS_OUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/viewer.fs)
set(BVH_SHADER_VS_OUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/bvh_view.vs)
set(BVH_SHADER_FS_OUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/bvh_view.fs)
set(MORTON_SHADER_CS_OUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/bvh_morton.cs)
set(RADIXSORT_SHADER_CS_OUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/bvh_radix_sort.cs)
set(RADIXTREE_SHADER_CS_OUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/bvh_radix_tree.cs)
configure_file(${VIEW_SHADER_VS_IN} ${VIEW_SHADER_VS_OUT} COPYONLY)
configure_file(${VIEW_SHADER_FS_IN} ${VIEW_SHADER_FS_OUT} COPYONLY)
configure_file(${BVH_SHADER_VS_IN} ${BVH_SHADER_VS_OUT} COPYONLY)
configure_file(${BVH_SHADER_FS_IN} ${BVH_SHADER_FS_OUT} COPYONLY)
configure_file(${MORTON_SHADER_CS_IN} ${MORTON_SHADER_CS_OUT} COPYONLY)
configure_file(${RADIXSORT_SHADER_CS_IN} ${RADIXSORT_SHADER_CS_OUT} COPYONLY)
configure_file(${RADIXTREE_SHADER_CS_IN} ${RADIXTREE_SHADER_CS_OUT} COPYONLY)


include_directories(
	${GLFW_INCLUDE_PATH}
	${GLEW_INCLUDE_PATH}
	${ASSIMP_INCLUDE_PATH}
	${GLM_INCLUDE_PATH}
)
add_executable(RayTracingGPUE
	${HEADER_FILES}
	${SOURCE_FILES}
)
target_link_libraries(RayTracingGPUE
	${OPENGL_LIBRARY}
	${GLFW_LIBRARY}
	${ASSIMP_LIBRARY}
	${GLEW_LIBRARY}
)