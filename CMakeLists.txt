cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0037 OLD)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
##############################################################################

project(eea)

# All demos
#option(MYCGAL_ENABLED "Run the program with Cgal" OFF)

set(MYCGAL_ENABLED 0)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()


if(MYCGAL_ENABLED)
    find_package(GMP REQUIRED)
    find_package(CGAL QUIET REQUIRED)
    set(CGAL_DONT_OVERRIDE_CMAKE_FLAGS TRUE CACHE BOOL "CGAL's CMAKE Setup is super annoying ")
endif(MYCGAL_ENABLED)

add_subdirectory(ext ext_build)

include_directories(

# OpenEXR high dynamic range bitmap library
${OPENEXR_INCLUDE_DIRS}

# Intel Thread Building Blocks
${TBB_INCLUDE_DIR}

${GMP_INCLUDE_DIR}
ext
src/core
src/analyzer
src/integrand
src/sampler
)

if(MYCGAL_ENABLED)
FILE ( GLOB SOURCE
src/core/*.cpp
src/analyzer/*.cpp
src/integrand/*.cpp
src/sampler/*.cpp
src/main/main.cpp
)
FILE ( GLOB HEADERS
src/core/*.h
src/analyzer/*.h
src/integrand/*.h
src/sampler/*.h
)
else()
    FILE ( GLOB SOURCE
    src/core/*.cpp
    src/analyzer/*.cpp
    src/sampler/*.cpp
    src/main/main.cpp
    src/integrand/DiskIntegrand.cpp
    src/integrand/GaussianIntegrand.cpp
    src/integrand/integrand.cpp
    src/integrand/PBRTIntegrand.cpp
    )
    FILE ( GLOB HEADERS
    src/core/*.h
    src/analyzer/*.h
    src/sampler/*.h
    src/integrand/DiskIntegrand.h
    src/integrand/GaussianIntegrand.h
    src/integrand/integrand.h
    src/integrand/PBRTIntegrand.h
    )
endif(MYCGAL_ENABLED)

set(EEA_DEPS_LIBRARIES
  tbb_static
  IlmImf
)


add_executable(eea ${SOURCE} ${HEADERS})

if(MYCGAL_ENABLED)
    TARGET_LINK_LIBRARIES(eea blas ${GMP_LIBRARIES} ${OPENEXR_LIBS} ${EEA_DEPS_LIBRARIES})
else()
    TARGET_LINK_LIBRARIES(eea ${OPENEXR_LIBS} ${EEA_DEPS_LIBRARIES})
endif(MYCGAL_ENABLED)

install(TARGETS eea RUNTIME DESTINATION bin)
