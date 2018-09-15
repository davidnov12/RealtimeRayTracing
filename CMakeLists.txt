cmake_minimum_required (VERSION 3.3)

project(GPUEExample)

find_package(OpenGL REQUIRED)

set(GLFW_INCLUDE_PATH "" CACHE PATH "glfw include directory" )
set(GLFW_LIBRARY "" CACHE FILEPATH "libglfw.so or glfw.lib" )
set(GLFW_DYNAMIC_LIBRARY "" CACHE FILEPATH "libglfw.a or glfw.dll" )

set(GPUE_INCLUDE_PATH "" CACHE PATH "gpue include directory" )
set(GEGL_LIBRARY "" CACHE FILEPATH "gegl static lib" )
set(GEGL_DYNAMIC_LIBRARY "" CACHE FILEPATH "gegl dynamic lib" )

set(SOURCE_FILES
	main.cpp
)

string(FIND ${GLFW_DYNAMIC_LIBRARY} "/" pozice REVERSE)
string(LENGTH ${GLFW_DYNAMIC_LIBRARY} delka)
string(CONCAT exp ${delka} "-" ${pozice} "-1")
math(EXPR pos ${exp})
string(CONCAT pozice ${pozice} "+1")
math(EXPR pozice ${pozice})
string(SUBSTRING ${GLFW_DYNAMIC_LIBRARY} ${pozice} ${pos} filename)
set(GLFW_DYN ${CMAKE_CURRENT_BINARY_DIR}/${filename})

string(FIND ${GEGL_DYNAMIC_LIBRARY} "/" pozice REVERSE)
string(LENGTH ${GEGL_DYNAMIC_LIBRARY} delka)
string(CONCAT exp ${delka} "-" ${pozice} "-1")
math(EXPR pos ${exp})
string(CONCAT pozice ${pozice} "+1")
math(EXPR pozice ${pozice})
string(SUBSTRING ${GEGL_DYNAMIC_LIBRARY} ${pozice} ${pos} filename)
set(GEGL_DYN ${CMAKE_CURRENT_BINARY_DIR}/${filename})


configure_file(${GLFW_DYNAMIC_LIBRARY} ${GLFW_DYN} COPYONLY)
configure_file(${GEGL_DYNAMIC_LIBRARY} ${GEGL_DYN} COPYONLY)


include_directories(
	${GLFW_INCLUDE_PATH}
	${GPUE_INCLUDE_PATH}
)
add_executable(RayTracingGPUE
	${SOURCE_FILES}
)
target_link_libraries(RayTracingGPUE
	${OPENGL_LIBRARY}
	${GLFW_LIBRARY}
	${GEGL_LIBRARY}
)