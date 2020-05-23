build:
	gcc -Wall \
	./src/mesh.c ./src/triangle.c ./src/vector.c ./src/display.c ./src/main.c \
	-std=c99 \
	-I"C:\courses\pikuma3d\3drenderer\libsdl\include" \
	-L"C:\courses\pikuma3d\3drenderer\libsdl\lib" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-o ./bin/example.exe
	
run:
	./bin/example.exe
