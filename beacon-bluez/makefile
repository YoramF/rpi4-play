CFILES = $(wildcard ./src/*.c)
OFILES = $(CFILES:src/%.c=obj/%.o)
OFILES_D = $(CFILES:src/%.c=obj/%_d.o)
GCCFLAGS = -O3 -s
GCCFLAGS_D = -g

beacon:	$(OFILES)
	gcc $(GCCFLAGS) -o ./bin/beacon $(OFILES) -lbluetooth

beacon_d: $(OFILES_D)
	gcc $(GCCFLAGS_D) -o ./bin/beacon_d $(OFILES_D) -lbluetooth
 
./obj/%.o: ./src/%.c
	gcc $(GCCFLAGS) -c $< -o $@

./obj/%_d.o: ./src/%.c
	gcc $(GCCFLAGS_D) -c $< -o $@

clean:
	rm ./obj/*.o