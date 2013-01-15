
set(testmodel_SRCS
    testmodel.cpp
)
set(testmodel_HDRS
    testmodel.h
)

set(QT_USE_QTTEST TRUE)

QT4_WRAP_CPP(testmodel_HEADERS_MOC ${testmodel_HDRS})

add_executable(testmodel ${testmodel_SRCS} ${testmodel_HEADERS_MOC})
target_link_libraries(testmodel ${QT_LIBRARIES})