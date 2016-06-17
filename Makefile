all:
	g++ main.cpp -o main
	./main -f file_list -v vocab -i invIndex -q queryFile