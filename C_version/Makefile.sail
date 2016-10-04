CC = gcc

CFLAGS = -Ofast -I/usr/include/SDL2 -DSELFISHOS -DHOMEDIR
#-DENABLE_OPENAL -Igorilla-audio/include
LDFLAGS = -lSDL2 -lm -s -flto
DEFINES = -Wall
OUTPUT = harbour-rubbybird100

SOURCES = main.c wrapper_sail.c
#./gorilla-audio/src/ga.c \
#./gorilla-audio/src/gau.c \
#./gorilla-audio/src/ga_stream.c \
#./gorilla-audio/src/devices/ga_openal.c \
#./gorilla-audio/src/common/gc_common.c \
#./gorilla-audio/src/common/gc_thread.c
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${OBJS} ${CFLAGS} ${LDFLAGS} ${DEFINES} 
	
clean:
	rm main.o wrapper_sail.o ${OUTPUT}

install:
	chmod +x ${OUTPUT}
	mkdir /home/deploy/installroot
	mkdir /home/deploy/installroot/usr
	mkdir /home/deploy/installroot/usr/bin
	mkdir /home/deploy/installroot/usr/share
	mkdir /home/deploy/installroot/usr/share/harbour-rubbybird100
	mkdir /home/deploy/installroot/usr/share/harbour-rubbybird100/data
	mkdir /home/deploy/installroot/usr/share/applications
	mkdir /home/deploy/installroot/usr/share/icons
	mkdir /home/deploy/installroot/usr/share/icons/hicolor
	mkdir /home/deploy/installroot/usr/share/icons/hicolor/86x86
	mkdir /home/deploy/installroot/usr/share/icons/hicolor/86x86/apps
	cp ./$(OUTPUT) /home/deploy/installroot/usr/bin/$(OUTPUT)
	cp ./killminds.desktop /home/deploy/installroot/usr/share/applications/harbour-rubbybird100.desktop
	cp ./data/86x86.png /home/deploy/installroot/usr/share/icons/hicolor/86x86/apps/harbour-rubbybird100.png
	cp ./data/alarm.wav /home/deploy/installroot/usr/share/harbour-rubbybird100/data/alarm.wav
	cp ./data/bird.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/bird.bmp
	cp ./data/chop.ogg /home/deploy/installroot/usr/share/harbour-rubbybird100/data/chop.ogg
	cp ./data/ddot.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/ddot.bmp
	cp ./data/dies.wav /home/deploy/installroot/usr/share/harbour-rubbybird100/data/dies.wav
	cp ./data/fond2.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/fond2.bmp
	cp ./data/fond.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/fond.bmp
	cp ./data/go.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/go.bmp
	cp ./data/inst.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/inst.bmp
	cp ./data/left.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/left.bmp
	cp ./data/limit_score.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/limit_score.bmp
	cp ./data/logo.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/logo.bmp
	cp ./data/rubby.ogg /home/deploy/installroot/usr/share/harbour-rubbybird100/data/rubby.ogg
	cp ./data/score.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/score.bmp
	cp ./data/tap.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/tap.bmp
	cp ./data/tick.wav /home/deploy/installroot/usr/share/harbour-rubbybird100/data/tick.wav
	cp ./data/tweet.bmp /home/deploy/installroot/usr/share/harbour-rubbybird100/data/tweet.bmp
