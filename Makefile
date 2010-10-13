ifeq ($(shell uname), Darwin)
	P3DINCLUDEDIR := /Developer/Panda3D/include
	P3DLIBDIR := /Developer/Panda3D/lib
else
	P3DINCLUDEDIR := /usr/include/panda3d
	P3DLIBDIR := /usr/lib/panda3d
endif

ProjectHousefire: main.o
	g++ main.o -o ProjectHousefire -fPIC -L$(P3DLIBDIR) -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3dtool -lp3pystub -lp3direct
main.o: src/main.cpp
	g++ -c src/main.cpp -o main.o -fPIC -O2 -I/usr/include/python2.6 -I$(P3DINCLUDEDIR)
clean:
	rm main.o ProjectHousefire
