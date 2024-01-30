#! /bin/sh

OS=$(uname -o)

cd build;

if [ "$OS" = "Msys" ]; then
    ./opengl_space_odyssey.exe;
elif [ "$OS" = "GNU/Linux" ]; then
    ./opengl_space_odyssey;
fi