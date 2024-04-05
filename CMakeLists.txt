project(ChessBoard)


find_package(SDL2 REQUIRED)

file( GLOB SRCS *.cpp *.h *.hpp )

add_executable(app ${SRCS}
    Interface.h Interface.cpp)
target_include_directories(app PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(app ${SDL2_LIBRARIES} SDL2_ttf SDL2_image)
