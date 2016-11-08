cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0037 OLD)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules/")
##############################################################################

project(eea)

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

find_package(TBB REQUIRED)

SET(ILMBASE_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
SET(OPENEXR_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
SET(OPENEXR_BUILD_SHARED_LIBS    OFF CACHE BOOL " " FORCE)
SET(ILMBASE_BUILD_SHARED_LIBS    OFF CACHE BOOL " " FORCE)
ADD_SUBDIRECTORY(ext/openexr)
SET_PROPERTY(TARGET IexMath eLut toFloat b44ExpLogTable dwaLookups CopyIlmBaseLibs IlmThread Half Iex Imath IlmImf PROPERTY FOLDER "dependencies")
INCLUDE_DIRECTORIES (
  ext/openexr/IlmBase/Imath
  ext/openexr/IlmBase/Half
  ext/openexr/IlmBase/Iex
  ext/openexr/OpenEXR/IlmImf
  ${CMAKE_BINARY_DIR}/ext/openexr/IlmBase/config
  ${CMAKE_BINARY_DIR}/ext/openexr/OpenEXR/config
)

SET(OPENEXR_LIBS IlmImf Imath Half)

include_directories(
${TBB_INCLUDE_DIRS}
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

TARGET_LINK_LIBRARIES(eea blas gmp ${OPENEXR_LIBS} ${TBB_LIBRARIES})

install(TARGETS eea RUNTIME DESTINATION bin)
