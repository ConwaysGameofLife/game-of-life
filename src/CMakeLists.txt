PROJECT(LifeGameScreenSaver)

find_package(wxWidgets REQUIRED COMPONENTS core base)

include(${wxWidgets_USE_FILE})

SET(src
    gameconfig.h
    ivlib.cpp
    ivlib.h
    lifegamepanel.cpp
    lifegamepanel.h
    random.h
    screensaver.cpp
    universe.cpp
    universe.h
    vax.cpp
    vax.h
)

ADD_EXECUTABLE(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
