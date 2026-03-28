cmake_minimum_required(VERSION 3.14)
project(trading_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compile flags for warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(trading_engine
    src/main.cpp
    src/benchmark.cpp
    src/orderbook.cpp
    src/trading_engine.cpp
)
