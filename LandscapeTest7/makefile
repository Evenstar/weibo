all : create_map.cpp paint.cpp pg_twitter.cpp create_idmap.cpp pg_weibo.cpp
	g++ -O2 -Wall -o create_map create_map.cpp
	g++ -O2 -Wall -o paint paint.cpp
	g++ -O2 -Wall -o rank rank.cpp
	g++ -O3 -Wall -o pg_twitter pg_twitter.cpp
	g++ -O3 -Wall -o pg_weibo pg_weibo.cpp
	g++ -O3 -Wall -o create_idmap create_idmap.cpp
	g++ -O3 -Wall -o cal weibo_calPagerank.cpp
clean:
	$(RM) *.o
	$(RM) create_map paint rank pg_weibo pg_twitter create_idmap
	$(RM) .depend 