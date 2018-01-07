PROJECT(gameoflife)

find_package(wxWidgets REQUIRED COMPONENTS core base)

include(${wxWidgets_USE_FILE})

include_directories(..)

SET(src
    app.cpp
    config.cpp
    config.h
    lifegamepanel.cpp
    lifegamepanel.h
)

configure_file(glider.png glider.png COPYONLY)

ADD_EXECUTABLE(${PROJECT_NAME} ${src})
target_link_libraries(${PROJECT_NAME} liblife ${wxWidgets_LIBRARIES})
