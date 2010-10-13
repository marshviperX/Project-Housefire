ProjectHousefire: main.o
	g++ main.o -o ProjectHousefire -fPIC -L/usr/lib/panda3d -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3dtool -lp3pystub -lp3direct
main.o: src/main.cpp
	g++ -c src/main.cpp -o main.o -fPIC -O2 -I/usr/include/python2.6 -I/usr/include/panda3d/
clean:
	rm main.o ProjectHousefire
