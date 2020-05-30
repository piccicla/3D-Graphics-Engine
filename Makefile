build:
	gcc -Wall \
	./src/*.c \
	-std=c99 \
	-I"C:\courses\pikuma3d\3drenderer\libsdl\include" \
	-L"C:\courses\pikuma3d\3drenderer\libsdl\lib" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-o ./bin/example.exe
	
run:
	./bin/example.exe
