PROJECT(liblife)

SET(src
    bigbang.cpp
    bigbang.h
    cpuavxompuniverse.cpp
    cpuavxuniverse.h
    cpuippompuniverse.cpp
    cpuipptbb.cpp
    cpuippuniverse.cpp
    cpuomploopuniverse.cpp
    cpusinglethreadedloop.cpp
    gameconfig.h
    ivlib.cpp
    ivlib.h
    random.h
    universe.cpp
    universe.h
    vax.cpp
    vax.h
)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${src})
