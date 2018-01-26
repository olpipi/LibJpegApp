

jpegLibPath = /proj/ss7caf/members/xolepip/intel/jpeg-6b/
CFLAGS= -g -fsanitize=address

.PHONY: all jpegApp clean buildLibraries
all: clean buildLibraries jpegApp
app: clean jpegApp

buildLibraries :
	$(MAKE) clean -C $(jpegLibPath)
	$(MAKE) all -C $(jpegLibPath)


jpegApp: main.o libjpegwrapper.o
	g++ $(CFLAGS) -L$(jpegLibPath) -ljpeg main.o libjpegwrapper.o -o jpegApp

main.o: main.cpp 
	g++ $(CFLAGS) main.cpp -c -o main.o

libjpegwrapper.o: libjpegwrapper.cpp libjpegwrapper.h
	g++ $(CFLAGS) libjpegwrapper.cpp -c -o libjpegwrapper.o

clean:
	rm -f *.o
