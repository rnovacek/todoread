
cmake_minimum_required(VERSION 2.8)

project(todoread)

set(TARGETS "DESKTOP;HARMATTAN;BLACKBERRY")
set(TARGET "DESKTOP" CACHE STRING "Target platform, select one of DESKTOP, HARMATTAN, BLACKBERRY")
set_property(CACHE TARGET PROPERTY STRINGS ${TARGETS})

message(STATUS "Target=${TARGET}")

find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork QtTest QtDeclarative REQUIRED)

include(${QT_USE_FILE})
set(QT_USE_QTDECLARATIVE TRUE)
set(QT_USE_QTNETWORK TRUE)
add_definitions(${QT_DEFINITIONS})

if (${TARGET} STREQUAL "DESKTOP")
    add_definitions(-DTODOREAD_QTWIDGET)
    add_definitions(-DTODOREAD_OTHER)
elseif (${TARGET} STREQUAL "HARMATTAN")
    add_definitions(-DTODOREAD_QTQUICK)
    add_definitions(-DTODOREAD_HARMATTAN)
elseif (${TARGET} STREQUAL "BLACKBERRY")
    set(QNX_HOST "" CACHE STRING "QNX Host directory, defined in bbndk-env.sh")
    if (QNX_HOST STREQUAL "")
        if (DEFINED ENV{QNX_HOST})
            set(QNX_HOST $ENV{QNX_HOST})
        else (DEFINED ENV{QNX_HOST})
            message(FATAL_ERROR "QNX_HOST variable is not set, did you sourced bbndk-env.sh first?")
        endif (DEFINED ENV{QNX_HOST})
    endif (QNX_HOST STREQUAL "")
    message(STATUS "QNX Host directory: ${QNX_HOST}")

    set(DEBUG_TOKEN "" CACHE STRING "Debug token for BlackBerry development")
    if (DEBUG_TOKEN STREQUAL "")
        message(FATAL_ERROR "DEBUG_TOKEN is not set")
    endif (DEBUG_TOKEN STREQUAL "")

    set(DEVICE "" CACHE STRING "Device hostname or IP address")
    if (DEVICE STREQUAL "")
        message(WARNING "Device is not set, deployment won't work")
    endif (DEVICE STREQUAL "")

    set(DEVICE_PASSWORD "" CACHE STRING "Password for application deployment")
    if (DEVICE_PASSWORD STREQUAL "")
        message(WARNING "Device password is not set, deployment won't work")
    endif (DEVICE_PASSWORD STREQUAL "")

    include(cmake/blackberry-toolchain.cmake)

    add_definitions(-DTODOREAD_CASCADES)
    add_definitions(-DTODOREAD_BLACKBERRY)
else (${TARGET} STREQUAL "DESKTOP")
    message(FATAL_ERROR "Unknown target ${TARGET}")
endif (${TARGET} STREQUAL "DESKTOP")

add_subdirectory(src)
add_subdirectory(test)
