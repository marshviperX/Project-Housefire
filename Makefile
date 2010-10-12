Panda1: Panda1.o
	g++ Panda1.o -o Pan -fPIC -L/usr/lib/panda3d -lp3framework -lpanda -lpandafx -lpandaexpress -lp3dtoolconfig -lp3dtool -lp3pystub -lp3direct
Panda1.o: main.cpp
	g++ -c main.cpp -o Panda1.o -fPIC -O2 -I/usr/include/python2.6 -I/usr/include/panda3d/
clean:
	rm Panda1.o Pan
