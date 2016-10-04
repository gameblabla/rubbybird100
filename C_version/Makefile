CC = gcc

CFLAGS = -O2 -fno-omit-frame-pointer -I./WRAPPER -I/usr/include/SDL2 -DHOMEDIR
LDFLAGS = -lSDL2 -lm -lSDL2_mixer

DEFINES = -Wall
OUTPUT = rubby

SOURCES = main.c wrapper.c
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${OBJS} ${CFLAGS} ${LDFLAGS} ${DEFINES} 
	
clean:
	rm *.o
	rm ${OUTPUT}
