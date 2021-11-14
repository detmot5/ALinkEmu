
file(GLOB_RECURSE Sources CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE Headers CONFIGURE_DEPENDS *.hpp)

add_executable(${PROJECT_NAME} ${Sources} ${Headers})
target_link_libraries(${PROJECT_NAME} PRIVATE ALinkEmu)

find_package(Qt5 COMPONENTS Widgets  REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)