project1: main1.o graph1.o
	g++ project1/source/graph.o project1/source/main.o -o project1/source/a.out

project2: graph1.o main2.o graph2.o

graph1.o:
	g++ -c project1/source/graph.cpp -o project1/source/graph.o

main1.o:
	g++ -c project1/source/main.cpp -o project1/source/main.o


main2.o:
	g++ -c project2/source/main.cpp -o project2/source/main.o