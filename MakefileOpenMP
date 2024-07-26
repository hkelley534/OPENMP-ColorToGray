PRJ_NAME=color2gray_openmp

GCC=g++

OUTPUT=${PRJ_NAME}
SOURCES=${PRJ_NAME}.cpp
CCFLAGS= -g3 -O0 -fopenmp -lm

all: ${OUTPUT}
${OUTPUT}: ${SOURCES}
	${GCC} -o ${OUTPUT} ${CCFLAGS} ${SOURCES}
    
clean:
	rm -f ${OUTPUT}
    
