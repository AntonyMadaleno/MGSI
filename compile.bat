cd src
g++ -O3 -m64 -IC:\Strawberry\c\include -LC:\Strawberry\c\lib -g -o ../bin/main.exe main.cpp Transform.cpp Shader.cpp Curve.c Sphere.c Surface.c Vec.c Quaternion.c Object.c Matrix.c Image.c Buffer.cpp Skybox.cpp Texture.cpp -lglfw3 -lglew32 -lgdi32 -lopengl32 -lpthread
pause
cd ../
cls
