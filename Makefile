CC = g++
LIBS=-lGL -lglfw -lGLEW
CFLAGS = -g -std=c++11 -O3 -I include -I lib/glm -I lib/assimp/include

SRCS=main.cpp observer.cpp window.cpp looper.cpp shader.cpp shaderprogram.cpp
OBJS = $(addprefix obj/,$(SRCS:.cpp=.o))

all: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS) $(LIBS)
obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(OBJS) $(MAIN)