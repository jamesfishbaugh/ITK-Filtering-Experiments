cmake_minimum_required(VERSION 3.10.2)

project(FilteringExperiments)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})


add_executable(FilteringExperiments FilteringExperiments.cxx)
target_link_libraries(FilteringExperiments ${ITK_LIBRARIES})
