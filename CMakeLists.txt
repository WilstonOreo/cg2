CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(cg2ss12)

INCLUDE_DIRECTORIES(./include ./TbdLib/include)

AUX_SOURCE_DIRECTORY(./src/lib cg2_src)

ADD_LIBRARY(cg2 STATIC ${cg2_src})

ADD_EXECUTABLE(cg2ex1 ./src/cg2ex1/main.cpp)

TARGET_LINK_LIBRARIES(cg2 glut GLU GL)
TARGET_LINK_LIBRARIES(cg2ex1 cg2)

#Tests
ADD_EXECUTABLE(mediantest ./src/test/mediantest.cpp)
TARGET_LINK_LIBRARIES(mediantest boost_program_options cg2)


#ADD_DEFINITIONS(-g -O2 -fsigned-char -freg-struct-return -Wall -W -Wshadow -Wpointer-arith -Wcast-qual -Winline)
ADD_DEFINITIONS(-g -O3 -fsigned-char -fopenmp -freg-struct-return -Wall -fPIC)

