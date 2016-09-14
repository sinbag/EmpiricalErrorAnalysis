cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0037 OLD)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
##############################################################################

project(eea)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(OpenMP)
if(OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

#find_package(TBB)

include_directories(
ext
src/core
src/analyzer
src/integrand
src/sampler
)

FILE ( GLOB SOURCE
src/core/*.cpp
src/analyzer/*.cpp
src/integrand/*.cpp
src/sampler/*.cpp
src/main/main.cxx
)

FILE ( GLOB HEADERS
src/core/*.h
src/analyzer/*.h
src/integrand/*.h
src/sampler/*.h
)

add_executable(eea ${SOURCE} ${HEADERS})

if(APPLE)
TARGET_LINK_LIBRARIES(eea blas CGAL gmp ) #${TBB_LIBRARIES}
else()
TARGET_LINK_LIBRARIES(eea blas gomp CGAL gmp ) #${TBB_LIBRARIES}
endif(APPLE)

install(TARGETS eea RUNTIME DESTINATION bin)
