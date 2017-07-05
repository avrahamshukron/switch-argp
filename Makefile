objs = switch-cli.o switch.o tcp.o

switch: $(objs)

.PHONY: clean

clean:
	rm -v switch $(objs)

all: switch