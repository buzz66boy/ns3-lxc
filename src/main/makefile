makeproject: main.cpp settings_conf.h settings_conf.cpp topology.h topology.cpp
	g++ -o main main.cpp settings_conf.cpp topology.cpp

run: makeproject
	./main ../../resources/example_topologies/simple_network.top

val: makeproject
	valgrind --track-origins=yes ./main ../../resources/example_topologies/simple_network.top

clean:
	$(RM) main
