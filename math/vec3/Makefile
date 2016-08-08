VEC3_SRCS = vec3.cpp testVec3.cpp

CC = g++
FLAGS = -g

default: clean build run

build : ${VEC3_SRCS}
	${CC} ${FLAGS} ${VEC3_SRCS} -o testVec3

run : 
	./testVec3

clean : 
	rm -f core *~ *.o TAGS testVec3
