CFILES = $(wildcard ./src/*.c)
OFILES = $(CFILES:src/%.c=obj/%.o)
OFILES_D = $(CFILES:src/%.c=obj/%_d.o)
GCCFLAGS = -O3 -s
GCCFLAGS_D = -g

bluetooth:	$(OFILES)
	gcc $(GCCFLAGS) -o ./bin/bluetooth $(OFILES)

bluetooth_d: $(OFILES_D)
	gcc $(GCCFLAGS_D) -o ./bin/bluetooth_d $(OFILES_D)

./obj/%.o: ./src/%.c
	gcc $(GCCFLAGS) -c $< -o $@

./obj/%_d.o: ./src/%.c
	gcc $(GCCFLAGS_D) -c $< -o $@

clean:
	rm ./obj/*.o