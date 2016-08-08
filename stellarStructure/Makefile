BSTAR_SRCS = \
	../math/interpolate.cpp \
	opalTable.cpp eosTables.cpp ktTables.cpp \
	mltSolver.cpp photosphere.cpp nuclear.cpp \
	structureScalars.cpp starBuilder.cpp \
	bstar.cpp

CC = g++
FLAGS = -g
FLAGSO3 = -O3
INCLUDES = -I../misc -I../math -I../math/vec3 
LIBS = -llapack_atlas -llapack -lblas -latlas -lm

all: bstar
default: bstar

bstar : ${BSTAR_SRCS}
	${CC} ${FLAGS} ${INCLUDES} ${BSTAR_SRCS} -o bstar ${LIBS} 

bstarO3 : ${BSTAR_SRCS}
	${CC} ${FLAGSO3} ${INCLUDES} ${BSTAR_SRCS} -o bstarO3 ${LIBS} 

clean : 
	rm -f core *~ *.o bstar bstarO3

