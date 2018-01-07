CXX_FLAG = -ggdb

project1: main1.o graph1.o
	g++ $(CXX_FLAG) project1/source/graph.o project1/source/main.o -o project1/source/a.out

project2: graph1.o main2.o graph2.o
	g++ $(CXX_FLAG) project1/source/graph.o project2/source/main.o project2/source/graph.o -o project2/source/a.out

graph1.o:
	g++ $(CXX_FLAG) -c project1/source/graph.cpp -o project1/source/graph.o

main1.o:
	g++ $(CXX_FLAG) -c project1/source/main.cpp -o project1/source/main.o

main2.o:
	g++ $(CXX_FLAG) -c project2/source/main.cpp -o project2/source/main.o

graph2.o:
	g++ $(CXX_FLAG) -c project2/source/graph.cpp -o project2/source/graph.o