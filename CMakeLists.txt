cmake_minimum_required(VERSION 3.16)

project(IVT_deamon VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(DSKIP_BUILD_TEST on)

add_compile_options(-g -O0 -W)

add_executable(${PROJECT_NAME}
    main.cpp
    src/FileParser/FileParser.h
    src/FileParser/FileParser.cpp
    src/TimerHandlerSql/TimerHandlerSql.h
    src/TimerHandlerSql/TimerHandlerSql.cpp
    src/TimerHandler/TimerHandler.h
    src/TimerHandler/TimerHandler.cpp
    src/TimerHandlerDump/TimerHandlerDump.h
    src/TimerHandlerDump/TimerHandlerDump.cpp
    src/DbConnectionInfo/DbConnectionInfo.h
    src/DbConnectionInfo/DbConnectionInfo.cpp
    src/Logger/Logger.h
    src/Logger/Logger.cpp
)

add_subdirectory(libpqxx build-pqxx)
target_link_libraries(${PROJECT_NAME} -lpqxx -lpq -lpthread)
