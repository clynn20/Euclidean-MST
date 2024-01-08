CC = g++ -std=c++11
exe_file = mst
$(exe_file): euclideanmst.o 
	     $(CC) euclideanmst.o -o $(exe_file)
euclideanmst.o: euclideanmst.cpp
	     $(CC) -c euclideanmst.cpp
clean:	 
	rm -f *.out *.o $(exe_file)