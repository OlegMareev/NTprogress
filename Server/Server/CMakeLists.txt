cmake_minimum_required(VERSION 3.14)

project(Server LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

add_executable(Server
  main.cpp
  server.h server.cpp
  order.h order.cpp
  client.h client.cpp
  tradingfloor.h tradingfloor.cpp
  historyordersdb.h historyordersdb.cpp

  testing_tradingfloor.h testing_tradingfloor.cpp
)
target_link_libraries(Server Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(Server Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(Server Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(Server Qt${QT_VERSION_MAJOR}::Test)

install(TARGETS Server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
