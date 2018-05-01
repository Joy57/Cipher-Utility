
cipher: cipher.o 
	g++ cipher.o -o cipher -lcrypto

cipher.o: cipher.cpp
	g++ -c cipher.cpp

clean: 
	rm *.o cipher
