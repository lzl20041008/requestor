all: requestor

requestor: main.cpp
	g++ main.cpp -o requestor -O2
