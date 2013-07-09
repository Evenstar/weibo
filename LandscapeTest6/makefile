all : create_map.cpp paint.cpp pagerank.cpp create_strict_map.cpp
	g++ -O2 -Wall -o create_map create_map.cpp
	g++ -O2 -Wall -o paint paint.cpp
	g++ -O2 -Wall -o rank rank.cpp
clean:
	$(RM) *.o
	$(RM) create_map paint rank
	$(RM) .depend 