
set(todoread_LIBS ${QT_LIBRARIES})

set(todoread_SRCS
    controller.cpp
    downloadthread.cpp
    json.cpp
    main.cpp
    readdatasource.cpp
    readitlater.cpp
    todoread.cpp
    todoreaditem.cpp
    todoreadmodel.cpp
)

set(todoread_HDRS
    controller.h
    downloadthread.h
    readdatasource.h
    readitlater.h
    todoread.h
    todoreaditem.h
    todoreadmodel.h
)

if (${TARGET} STREQUAL "HARMATTAN")
    set(todoread_SRCS
        ${todoread_SRCS}
        identitymanager.cpp
    )
    set(todoread_HDRS
        ${todoread_HDRS}
        identitymanager.h
    )
endif (${TARGET} STREQUAL "HARMATTAN")

if (${TARGET} STREQUAL "BLACKBERRY")
    set(todoread_SRCS
        ${todoread_SRCS}
        bbreadmodel.cpp
    )
    set(todoread_HDRS
        ${todoread_HDRS}
        bbreadmodel.h
    )
    set(todoread_LIBS
        ${todoread_LIBS}
        -lbb -lbbcascades
    )
endif (${TARGET} STREQUAL "BLACKBERRY")

QT4_WRAP_CPP(todoread_HEADERS_MOC ${todoread_HDRS})

add_executable(todoread ${todoread_SRCS} ${todoread_HEADERS_MOC})
target_link_libraries(todoread ${todoread_LIBS})

if (${TARGET} STREQUAL "BLACKBERRY")
    configure_file(bar-descriptor.xml.in bar-descriptor.xml)
    add_custom_target(pack
        ${QNX_HOST}/usr/bin/blackberry-nativepackager -devMode -debugToken ${DEBUG_TOKEN} -package todoread.bar bar-descriptor.xml
        DEPENDS todoread
    )

    add_custom_target(deploy
        ${QNX_HOST}/usr/bin/blackberry-deploy -installApp -device ${DEVICE} -password ${DEVICE_PASSWORD} todoread.bar
        DEPENDS pack
    )

    configure_file(bb-debug.sh.in bb-debug.sh)
    add_custom_target(debug
        sh bb-debug.sh
        DEPENDS deploy
    )
endif (${TARGET} STREQUAL "BLACKBERRY")

# home/radek/bbndk/host_10_0_9_404/linux/x86/usr/bin/blackberry-nativepackager" -devMode -debugToken /home/radek/bbndk/debugtoken.bar -package /home/radek/app/todoread/todoread-build-BlackBerry_10-Default_build_Debug/src/todoread.bar /home/radek/app/todoread/todoread-build-BlackBerry_10-Default_build_Debug/bar-descriptor_bz4821.xml
#/home/radek/bbndk/host_10_0_9_404/linux/x86/usr/bin/blackberry-deploy" -installApp -device 192.168.1.104 -password <hidden> /home/radek/app/todoread/todoread-build-BlackBerry_10-Default_build_Debug/src/todoread.bar