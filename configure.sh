#!/bin/bash

for i in `ls -1 src/`  ; do
  if [[ "$i" != "include" ]]; then
    g++ -g -Isrc/include -std=c++20 -c src/$i -o build/${i%.cpp}.o
  fi
done

OBJS=""

for i in `ls -1 build/` ; do 
  if [[ "$i" != "karui" ]]; then
    OBJS="$OBJS build/$i"
  fi
done

g++ -g $OBJS -o build/karui -lyaml
rm $OBJS
