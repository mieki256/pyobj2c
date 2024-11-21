MODELS = cube01.h suzanne01.h car.h scooter.h
SRCS = 01_drawobj.c $(MODELS)

ifeq ($(OS),Windows_NT)
# Windows
TARGET = 01_drawobj.exe
GCC_VERSION=$(shell gcc -dumpversion)

ifeq ($(GCC_VERSION),6.3.0)
# MinGW gcc 6.3.0
LIBS = -static -lopengl32 -lglu32 -lwinmm -lgdi32 -lglfw3dll -mwindows
else
# MinGW gcc 9.2.0, MSYS2
LIBS = -static -lopengl32 -lglu32 -lwinmm -lgdi32 -lglfw3 -mwindows
endif

else
# Linux (Ubuntu Linux 22.04 LTS, gcc 11.4.0)
TARGET = 01_drawobj
LIBS = -lGL -lGLU -lglfw -lm
endif

$(TARGET): $(SRCS) $(MODELS) Makefile
	gcc -o $@ $(SRCS) $(LIBS)

%.h: models/%.obj
	python ../pyobj2c.py $< > $@

.PHONY: clean
clean:
	rm -f *.o $(TARGET) $(MODELS)
