all: plainsight
plainsight: plainsight.o bmp.o
		@-gcc plainsight.o bmp.o -g -Wall -std=c99 -o "plainsight"
plainsight.o: plainsight.c bmp.h
		@-gcc -c "plainsight.c" -g -Wall -std=c99
bmp.o: bmp.c bmp.h
		@-gcc -c "bmp.c" -g -Wall -std=c99
clean :
		@-rm "plainsight.o" "plainsight"