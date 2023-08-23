set(target_name "demo")

find_package(OpenSceneGraph COMPONENTS osg osgViewer OpenThreads osgAnimation osgGA osgDB REQUIRED QUIET)
add_executable(${target_name} "main.cpp")

if(OpenSceneGraph_FOUND)
    message("------------ osg found ------------")
    message("------------ ${OSG_LIBRARY}")
    message("------------ ${OSG_LIBRARIES}")
    message("------------ ${OSG_INCLUDE_DIR}")
    message("------------ ${OPENSCENEGRAPH_LIBRARIES}")
else()
    message("------------ osg not found ------------")
endif()

target_include_directories(${target_name} PRIVATE ${OSG_INCLUDE_DIR})
target_link_libraries(${target_name} PRIVATE ${OPENSCENEGRAPH_LIBRARIES})
