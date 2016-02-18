project(diskcheck)

find_package(Boost COMPONENTS system program_options filesystem REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-c++11-extensions")

add_executable(diskcheck main.cpp)
target_link_libraries(diskcheck
	${Boost_LIBRARIES})

include_directories(${diskcheck_INCLUDE_DIRS})
