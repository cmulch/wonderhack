all: plainsight
plainsight: plainsight.o
		@-gcc "plainsight.o" -g -Wall -std=c99 -o "plainsight"
plainsight.o: plainsight.c
		@-gcc -c "plainsight.c" -g -Wall -std=c99
clean :
		@-rm "plainsight.o" "plainsight"