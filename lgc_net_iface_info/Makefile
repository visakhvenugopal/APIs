#Makefile for Katadyn Desalination Products

UTIL := lgc-get-net-iface-info

SRCS := lgc_net_iface_info.c \
	main.c
INCDIRS = -Iinclude
OBJS := $(SRCS:%.c=build/%.o)
C_FLAGS = $(INCDIRS) -DDEBUG -DDEBUG_LVL=2 -Wall
LDFLAGS = $(LDPATHS) -DDEBUG -Wall
CC := gcc

all: build_dir $(UTIL)

build_dir:
	mkdir -p build/

$(UTIL): $(OBJS)
	echo "[LD] $(UTIL)"
	$(CC) -o build/$(UTIL) $(OBJS) $(LDFLAGS)

build/%.o: %.c
	echo "[CC]" $<
	$(CC) -c $(C_FLAGS) -o build/$*.o $<
clean:
	rm -rf build/
