cmake_minimum_required( VERSION 3.16.3 )
project( Pacman )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" )

find_package( SDL2 REQUIRED )
find_package( SDL2_image REQUIRED )

include_directories( "include/" ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_DIRS} )
file( GLOB SRC_FILES "src/*cpp" )

add_executable( Pacman ${SRC_FILES} )

target_link_libraries( Pacman ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} SDL2_ttf )
