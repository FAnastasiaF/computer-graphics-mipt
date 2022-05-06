# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (task1)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# 2triangle
add_executable(2triangle
		"2triangle/2triangle.cpp"
        common/shader.cpp
		common/shader.hpp

		"2triangle/blue.fragmentshader"
        "2triangle/red.fragmentshader"
		"2triangle/SimpleVertexShader.vertexshader"
		)
target_link_libraries(2triangle
		${ALL_LIBS}
		)

set_target_properties(2triangle PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/2triangle/")
create_target_launcher(2triangle WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/2triangle/")
create_default_target_launcher(2triangle WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/2triangle/") # tut 1 is not the default or people would complain that tut 2 doesn't work

# camera
add_executable(camera
		"camera/camera.cpp"
		common/shader.cpp
		common/shader.hpp

		"camera/blue.fragmentshader"
		"camera/red.fragmentshader"
		"camera/SimpleVertexShader.vertexshader"
		)
target_link_libraries(camera
		${ALL_LIBS}
		)

set_target_properties(camera PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/camera/")
create_target_launcher(camera WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/camera/")
create_default_target_launcher(camera WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/camera/") # tut 1 is not the default or people would complain that tut 2 doesn't work

# plambob
add_executable(plambob
		plambob/plambob.cpp
		common/shader.cpp
		common/shader.hpp
		common/controls.cpp
		common/controls.hpp
		common/texture.cpp
		common/texture.hpp

		plambob/TransformVertexShader.vertexshader
		plambob/ColorFragmentShader.fragmentshader
		)
target_link_libraries(plambob
		${ALL_LIBS}
		)
# Xcode and Visual working directories
set_target_properties(plambob PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/plambob/")
create_target_launcher(plambob WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/plambob/")


SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
	TARGET 2triangle POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/2triangle${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/2triangle/"
)
add_custom_command(
		TARGET camera POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/camera${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/camera/"
)

add_custom_command(
		TARGET plambob POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/plambob${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/plambob/"
)

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

