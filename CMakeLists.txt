cmake_minimum_required(VERSION 3.16)

project(PassManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)
find_package(OpenSSL REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        resources.qrc
)

#set(OPENSSL_ROOT_DIR "C:/Qt/Tools/OpenSSLv3/Win_x64")



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PassManager
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        #${SQLITE_SOURCES}
        credentials.h credentials.cpp
        crypto.h crypto.cpp
        form_hashvalidationfailed.h form_hashvalidationfailed.cpp form_hashvalidationfailed.ui
    )

else()
    if(ANDROID)
        add_library(PassManager SHARED
            ${PROJECT_SOURCES}
            #${SQLITE_SOURCES}
        )

    else()
        add_executable(PassManager
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(PassManager PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql ${OPENSSL_LIBRARIES})

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PassManager)
endif()
set_target_properties(PassManager PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PassManager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PassManager)
endif()

add_subdirectory(test)
