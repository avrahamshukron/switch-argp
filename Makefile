objs = switch-cli.o switch.o

switch: $(objs)

.PHONY: clean

clean:
	rm switch $(objs)

all: switch