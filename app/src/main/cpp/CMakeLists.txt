cmake_minimum_required(VERSION 3.22.1)

project("jitdemo")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVIXL_INCLUDE_TARGET_A64 -DVIXL_CODE_BUFFER_MMAP -O3")
#tcc
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTCC_TARGET_ARM64")

enable_language(ASM)
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s)
include_directories("vixl")

file(GLOB_RECURSE VIXL
        "vixl/*.cc" "vixl/*.c" "vixl/*.cpp"
        "vixl/aarch64/*.cc" "vixl/aarch64/*.c" "vixl/aarch64/*.cpp"
        )

file(GLOB_RECURSE TCC
        "tcc/*.c"
        "tcc/lib/*.c"
        "tcc/lib/*.S"
        )

file(GLOB_RECURSE JIT
        "*.c" "*.cc" "*.cpp"
        )

add_library(${CMAKE_PROJECT_NAME} SHARED
        ${VIXL}
        ${JIT}
        ${ECC}
        )

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log)

## ------------------- tcc
#project("tcc")
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTCC_TARGET_ARM64 -O3")
#
#enable_language(ASM)
#set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS s)
#
#file(GLOB_RECURSE TCC
#        "tcc/*.c"
#        "tcc/lib/*.c"
#        "tcc/lib/*.S"
#        )
#
#add_executable(tcc
#        ${TCC}
#        )
#
#target_link_libraries(tcc
#        android
#        log)
