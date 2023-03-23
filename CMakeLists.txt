cmake_minimum_required(VERSION 3.22)
project(GobangProject)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_CXX_STANDARD 14)

set(EasyX_ROOT D:/python/VS2022/VC/Auxiliary/VS)
set(EasyX_INC  ${EasyX_ROOT}/include)
set(EasyX_LINK  ${EasyX_ROOT}/lib/x64)

include_directories(
        ${EasyX_INC}
)

link_directories(
        ${EasyX_LINK}
)
add_executable(GobangProject main.cpp Man.cpp Man.h AI.cpp AI.h Chess.cpp Chess.h ChessGame.cpp ChessGame.h)
#add_executable(GobangProject main.cpp)
target_link_libraries(GobangProject
        EasyXa
        EasyXw
        )