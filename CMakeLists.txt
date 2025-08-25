cmake_minimum_required(VERSION 3.16)

project(PdTermV1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontra os componentes do Qt necessários
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)

# Lista de fontes do projeto
set(PROJECT_SOURCES
    main.cpp
    PdTermmainterminal.cpp
    PdTermmainterminal.h
    PdTermmainterminal.ui
    worker.h
    worker.cpp
    pdtermserial.h
    pdtermserial.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

    qt_add_executable(PdTermV1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        pdtermxmodem.h pdtermxmodem.cpp
        icons.qrc
        pdterminalcontrol.h pdterminalcontrol.cpp
    )
else()
    if(ANDROID)
        add_library(PdTermV1 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(PdTermV1
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Vincula as bibliotecas necessárias
target_link_libraries(PdTermV1 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SerialPort
)
target_link_libraries(PdTermV1 PRIVATE Qt6::Core)
target_link_libraries(PdTermV1 PRIVATE Qt6::Core)

# Configurações específicas de plataforma
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PdTermV1)
endif()

set_target_properties(PdTermV1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Instalação
include(GNUInstallDirs)
install(TARGETS PdTermV1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PdTermV1)
endif()
