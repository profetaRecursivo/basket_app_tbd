cmake_minimum_required(VERSION 3.16)

project(Basquet_App VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# set(TS_FILES Basquet_App_es_BO.ts)

set(PROJECT_SOURCES
        # ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Basquet_App
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/main.cpp
        src/views/login/LoginWindow.h
        src/views/login/LoginWindow.cpp
        src/views/main/MainWindow.h
        src/views/main/MainWindow.cpp
        src/models/Auth/AuthManager.h 
        src/models/Auth/AuthManager.cpp
        src/models/Auth/DbManager.h 
        src/models/Auth/DbManager.cpp
        src/models/Auth/UIGetter.h
        src/models/Auth/UIGetter.cpp
        src/views/UIComponents.h
        src/models/Athlete/AthleteManager.h
        src/models/Athlete/AthleteManager.cpp
        src/views/athlete/RegisterAthleteWindow.h
        src/views/athlete/RegisterAthleteWindow.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Basquet_App APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    # qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Basquet_App SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Basquet_App
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Basquet_App PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


# Linkear PostgreSQL
find_package(PostgreSQL REQUIRED)
target_include_directories(Basquet_App PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(Basquet_App PRIVATE ${PostgreSQL_LIBRARIES})

# Configurar ODB ORM
find_program(ODB_EXECUTABLE odb)
if(NOT ODB_EXECUTABLE)
    message(FATAL_ERROR "ODB compiler not found. Please install ODB from AUR: yay -S odb")
endif()

# Buscar librerías ODB
find_library(ODB_LIBRARY NAMES odb)
find_library(ODB_PGSQL_LIBRARY NAMES odb-pgsql)

if(NOT ODB_LIBRARY OR NOT ODB_PGSQL_LIBRARY)
    message(FATAL_ERROR "ODB libraries not found. Install: yay -S libodb libodb-pgsql")
endif()

message(STATUS "ODB compiler found: ${ODB_EXECUTABLE}")
message(STATUS "ODB library found: ${ODB_LIBRARY}")
message(STATUS "ODB PostgreSQL library found: ${ODB_PGSQL_LIBRARY}")

# Incluir headers de ODB
target_include_directories(Basquet_App PRIVATE /usr/include)

# Linkear librerías ODB
target_link_libraries(Basquet_App PRIVATE ${ODB_LIBRARY} ${ODB_PGSQL_LIBRARY})



# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Basquet_App)
endif()
set_target_properties(Basquet_App PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Basquet_App
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Basquet_App)
endif()
