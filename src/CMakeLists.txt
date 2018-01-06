PROJECT(gameoflife)

find_package(wxWidgets REQUIRED COMPONENTS core base)

include(${wxWidgets_USE_FILE})

SET(src
    app.cpp
    config.cpp
    config.h
    gameconfig.h
    ivlib.cpp
    ivlib.h
    lifegamepanel.cpp
    lifegamepanel.h
    random.h
    universe.cpp
    universe.h
    vax.cpp
    vax.h
)

ADD_EXECUTABLE(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
