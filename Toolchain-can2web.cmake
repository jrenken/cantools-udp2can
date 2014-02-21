# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Linux)

# which C and C++ compiler to use
SET(CMAKE_C_COMPILER   /home/jrenken/build/can2web/buildroot/build_arm_nofpu/staging_dir/bin/arm-linux-gcc)
SET(CMAKE_CXX_COMPILER /home/jrenken/build/can2web/buildroot/build_arm_nofpu/staging_dir/bin/arm-linux-g++)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  /home/jrenken/build/can2web/buildroot/build_arm_nofpu/staging_dir )

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

