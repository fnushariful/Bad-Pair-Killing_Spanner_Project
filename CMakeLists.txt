#project(BadPairsKilling)

#set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_CXX_FLAGS "-O3 -fopenmp -fexceptions -frounding-math -Wall -Wextra -Wno-unused")


#find_package(CGAL)

#add_executable(BadPairsKilling main.cpp PointGenOptions.h SortPairs.h ShortestPaths.h Utilities.h GreedySpanner.h point_partition.h StretchFactorExact.h Testing.h FindPerimeter.h FindNeighbors.h MergeNeighboringCells.h Modified_StretchFactorExperimental.h WSPD.h WSPD_TSpanner.h PerimeterKNearestNeighbor.h)
#target_link_libraries(BadPairsKilling gmp)
#target_link_libraries(BadPairsKilling mpfr)
#target_link_libraries(BadPairsKilling boost_system)
#target_link_libraries(BadPairsKilling boost_thread)
#target_link_libraries(BadPairsKilling CGAL)

cmake_minimum_required(VERSION 3.21)
project(BadPairsKilling)

set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.2.2/lib/cmake/")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

include_directories(/opt/homebrew/lib/)
#include_directories(/home/linuxbrew/.linuxbrew/include/)
#include_directories(/home/linuxbrew/.linuxbrew/Cellar/qt/6.2.2/include/)

include_directories(/opt/homebrew/Cellar)

#link_directories(/opt/homebrew/lib/)
link_directories(/opt/homebrew/Cellar/)

find_package(CGAL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Xpreprocessor -fopenmp -std=c++17 -fexceptions -Wall -Wextra -O3 -stdlib=libc++ -Wno-unused-parameter -Wno-macro-redefined")
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.2.2/lib/cmake/")
#set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt/6.2.2/QtWidgets/")

#set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/boost/1.76.0/lib/cmake")

add_executable(BadPairsKilling main.cpp PointGenOptions.h SortPairs.h ShortestPaths.h Utilities.h GreedySpanner.h point_partition.h StretchFactorExact.h Testing.h FindPerimeter.h FindNeighbors.h MergeNeighboringCells.h Modified_StretchFactorExperimental.h WSPD.h WSPD_TSpanner.h PerimeterKNearestNeighbor.h)

target_link_libraries(BadPairsKilling libgmp.a)
target_link_libraries(BadPairsKilling libmpfr.a)
#target_link_libraries(BadPairsKilling Qt6::Core)
#target_link_libraries(BadPairsKilling Qt6::Widgets)
target_link_libraries(BadPairsKilling boost_system)
target_link_libraries(BadPairsKilling boost_thread)