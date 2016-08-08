SRCS = trace.cpp binarySystem.cpp ray.cpp rayTracer.cpp traceStepRK4.cpp \
	../../math/vec3/vec3.cpp ../../math/interpolate.cpp
CC = g++
FLAGS = -g -I../../misc -I../../math -I../../math/vec3
FLAGSO3 = -O3 -I../../math -I../../math/vec3

all: trace
default: trace

trace : ${SRCS}
	${CC} ${FLAGS} ${SRCS} -o trace

traceO3 : ${SRCS}
	${CC} ${FLAGSO3} ${SRCS} -o traceO3

clean : 
	rm -f trace traceO3
