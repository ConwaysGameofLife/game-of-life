PROJECT(liblife)

SET(src
    gameconfig.h
    ivlib.cpp
    ivlib.h
    random.h
    universe.cpp
    universe.h
    vax.cpp
    vax.h
)

ADD_LIBRARY(${PROJECT_NAME} ${src})
