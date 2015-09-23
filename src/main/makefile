makeproject: main.cpp settings_conf.h settings_conf.cpp topology.h topology.cpp
	g++ -o main main.cpp settings_conf.cpp topology.cpp

run: makeproject
	./main

clean:
	$(RM) main
