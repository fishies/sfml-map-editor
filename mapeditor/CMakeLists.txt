cmake_minimum_required(VERSION 3.1)

project(mapeditor)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -static -lpthread -static-libgcc -static-libstdc++")

    set(SFML_STATIC_LIBRARIES TRUE)
    set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")

    set(TGUI_STATIC_LIBRARIES TRUE)
    set(TGUI_DIR "C:/TGUI-0.8/lib/cmake/TGUI")

    find_package(SFML 2.5 COMPONENTS graphics window network system REQUIRED)
    find_package(TGUI 0.8 REQUIRED)

    add_executable(${PROJECT_NAME} WIN32 "main.cpp" "tilepalette.cpp" "tilemap.cpp")
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-network sfml-system tgui)
endif(WIN32)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -lpthread")
    #set(SFML_STATIC_LIBRARIES TRUE)
    #set(TGUI_STATIC_LIBRARIES TRUE)

    #HAVE TO MANUALLY COMPILE SFML TO GET STATIC LIBRARIES FOR LINUX!

    find_package(SFML 2.5 COMPONENTS graphics window network system REQUIRED)
    find_package(TGUI 0.8 REQUIRED)

    add_executable(${PROJECT_NAME} "main.cpp" "tilepalette.cpp" "tilemap.cpp")
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window sfml-network sfml-system tgui)
endif(UNIX)
