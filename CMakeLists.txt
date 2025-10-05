cmake_minimum_required(VERSION 3.16)
project(mafia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-std=c++20)

add_executable(mafia
    main.cpp
    library/shared_ptr/shared_ptr.cpp
    library/unique_number_generator/unique_number_generator.cpp
    library/json/json.cpp
    game_classes/library/messages_queue.cpp
    game_classes/players/player_base.cpp
    game_classes/players/player_playable.cpp
    game_classes/players/player_civilian.cpp
    game_classes/players/player_doctor.cpp
)

add_custom_target(clean-build
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMENT "Очистка всех сгенерированных файлов"
)

add_custom_target(clean-all
    DEPENDS clean clean-build
    COMMENT "Очистка всех сгенерированных и бинарных файлов"
)
