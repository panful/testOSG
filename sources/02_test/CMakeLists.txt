set(target_name "02_demo")

find_package(OpenSceneGraph COMPONENTS osg osgViewer OpenThreads osgAnimation osgGA osgDB REQUIRED QUIET)
add_executable(${target_name} "main.cpp")

target_include_directories(${target_name} PRIVATE ${OSG_INCLUDE_DIR})
target_link_libraries(${target_name} PRIVATE ${OPENSCENEGRAPH_LIBRARIES})
