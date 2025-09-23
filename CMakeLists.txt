cmake_minimum_required(VERSION 3.16)
project(mafia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mafia
    main.cpp
    library/shared_ptr/shared_ptr.cpp
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
