#! /bin/sh

cd build;
make;

OS=$(uname -o)

if [ "$OS" = "Msys" ]; then

cp deps/glew/bin/* .
cp deps/freeglut/bin/* .

fi;

cd ..;