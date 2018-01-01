

train: getdata.o nn.o main.o
	g++ getdata.o nn.o main.o -o train -lm

getdata.o: getdata.cpp
	g++ -c getdata.cpp -lm

nn.o: nn.cpp
	g++ -c nn.cpp -lm

main.o: main.cpp
	g++ -c main.cpp


clean:
	rm *.o train
