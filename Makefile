CC = g++
LIBS=-lGL -lglfw -lGLEW -lassimp
CFLAGS = -g -std=c++14 -O3 -I include -I lib/glm -I src

SRCS := $(shell find src/ -name *.cpp -printf '%P ')
OBJS = $(addprefix obj/,$(SRCS:.cpp=.o))

all: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS) $(LIBS)
obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS) $(MAIN)