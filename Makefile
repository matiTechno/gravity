all:
	g++ -std=c++14 -O2 -Wall -Wextra -pedantic \
	main.cpp orthocamera.cpp shaderprogram.cpp \
	spriterenderer.cpp texture.cpp vao.cpp viewport.cpp \
	window.cpp \
	-I. \
	-lsfml-window -lsfml-system -lGLEW -lGL -lpthread \
	-o gravity
