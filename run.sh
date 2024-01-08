#!/bin/sh
mkdir build
cd build
cmake ..
cmake --build .
case `uname` in
  'Linux') 
  ./opengl_space_odyssey;;
  'WindowsNT')  
  ./opengl_space_odyssey.exe;;
  *)
  echo "unknown: $OSTYPE" ;;
esac
exit