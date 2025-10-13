cmake_minimum_required(VERSION 3.16)
project(mafia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_SANITIZER "Enable sanitizer" ON)
if(USE_SANITIZER)
    set(SANITIZER_FLAG1 "-fsanitize=address")
    set(SANITIZER_FLAG2 "-g")
    add_compile_options(${SANITIZER_FLAG1})
    add_compile_options(${SANITIZER_FLAG2})
    add_link_options(${SANITIZER_FLAG1})
    add_link_options(${SANITIZER_FLAG2})
    message(STATUS "Sanitizer enabled with flags: ${SANITIZER_FLAGS}")
else()
    message(STATUS "Sanitizer disabled")
endif()

add_compile_options(-std=c++20)

add_executable(main
    main.cpp
    library/shared_ptr/shared_ptr.cpp
    library/unique_number_generator/unique_number_generator.cpp
    library/json/json.cpp
    library/logger/logger.cpp
    library/args_parser/args_parser.cpp
    game_classes/library/messages_queue.cpp
    game_classes/library/roles_enum.cpp
    game_classes/players/player_base.cpp
    game_classes/players/player_playable.cpp
    game_classes/players/player_civilian.cpp
    game_classes/players/player_doctor.cpp
    game_classes/players/player_sheriff.cpp
    game_classes/players/player_maniac.cpp
    game_classes/players/player_mafia.cpp
    game_classes/players/player_leader.cpp
    game_classes/user/user_base.cpp
    game_classes/user/user_civilian.cpp
    game_classes/user/user_doctor.cpp
    game_classes/user/user_maniac.cpp
    game_classes/user/user_sheriff.cpp
    game_classes/user/user_mafia.cpp
)

add_custom_target(clean-build
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMENT "Очистка всех сгенерированных файлов"
)

add_custom_target(clean-logs
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/logs
    COMMENT "Очистка всех логов"
)

add_custom_target(clean-all
    DEPENDS clean clean-build clean-logs
    COMMENT "Очистка всех сгенерированных и бинарных файлов"
)
