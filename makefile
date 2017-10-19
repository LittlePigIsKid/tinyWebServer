#foo.out: src/core/sha.o src/cgi/cgi.o src/http/http.o src/http/connect.o
#	gcc src/core/sha.o src/cgi/cgi.o src/http/http.o src/http/connect.o -o foo.out 

foo.out: sha.o cgi.o http.o connect.o
	gcc sha.o cgi.o http.o connect.o -o foo.out 
cgi.o: src/cgi/cgi.c 
	gcc -c ./src/cgi/cgi.c  -I src/http/

sha.o: src/core/sha.c src/core/sha.h src/util/dbg.h
	gcc -c ./src/core/sha.c -I src/http/  -I src/util/

connect.o: src/http/connect.c 
	gcc -c ./src/http/connect.c 

http.o: src/http/http.c
	gcc -c ./src/http/http.c -I src/core/ 

clean: sha.o cgi.o connect.o http.o
	rm -f sha.o cgi.o connect.o http.o
