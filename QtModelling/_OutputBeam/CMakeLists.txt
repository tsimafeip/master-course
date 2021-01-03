cmake_minimum_required(VERSION 3.5)

project(_outputBeam LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(_outputBeam SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    predicates.h
    predicates.cpp
  )
else()
  add_executable(_outputBeam
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    predicates.h
    predicates.cpp
  )
endif()

target_link_libraries(_outputBeam PRIVATE Qt5::Widgets)
