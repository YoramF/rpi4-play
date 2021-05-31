CFILES = $(wildcard ./src/*.c)
OFILES = $(CFILES:./src/%.c=./obj/%.o)
OFILES_D = $(CFILES:./src/%.c=./obj/%_d.o)
GCCFLAGS = -O3
GCCFLAGS_D = -g

syncf: $(OFILES)
	gcc -s -o ./bin/syncf $(OFILES)

syncf_d: $(OFILES_D)
	gcc -o ./bin/syncf_d $(OFILES_D)

./obj/%.o: ./src/%.c
	gcc $(GCCFLAGS) -c $< -o $@

./obj/%_d.o: ./src/%.c
	gcc $(GCCFLAGS_D) -c $< -o $@

clean:
	rm -f ./bin/* ./obj/*.*