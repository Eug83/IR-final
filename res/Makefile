all:
	@# query.py usage: python3 query.py url queryTermNum
	@# main program usage: ./main -f file_list -v vocab -i invIndex -q query -t topResult

	g++ main.cpp -o main
	python3 query.py http://hsing16.pixnet.net/blog/post/33292894 20 > ../data/query
	./main -f ../data/file_list -v ../data/vocab -i ../data/invIndex -q ../data/query -t 15 > ../data/result