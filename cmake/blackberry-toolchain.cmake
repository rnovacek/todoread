include(CMakeForceCompiler)

# this one is important
set(CMAKE_SYSTEM_NAME Blackberry)

# specify the cross compiler
CMAKE_FORCE_CXX_COMPILER(${QNX_HOST}/usr/bin/qcc GNU)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH ${QNX_HOST})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Vgcc_ntoarmv7le -lang-c++ -mcpu=cortex-a9 -Wno-psabi  -fstack-protector-strong")
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -Vgcc_ntoarmv7le -lang-c++")


# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)