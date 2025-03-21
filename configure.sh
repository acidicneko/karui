#!/bin/bash
if [ ! -f /usr/local/lib/libcjson.so ]; then
  gcc -c -pedantic -Wall -Werror -Wstrict-prototypes -Wwrite-strings -Wshadow -Winit-self -Wcast-align -Wformat=2 -Wmissing-prototypes -Wstrict-overflow=2 -Wcast-qual -Wc++-compat -Wundef -Wswitch-default -Wconversion -fstack-protector -fPIC cJSON/cJSON.c -o build/cJSON.o
  gcc -shared -o build/libcjson.so build/cJSON.o -Wl,-soname=libcjson.so

  mkdir -p /usr/local/include/cJSON
  cp -a cJSON/cJSON.h /usr/local/include/cJSON/cJSON.h

  cp -a build/libcjson.so /usr/local/lib/libcjson.so

  ldconfig

  rm build/cJSON.o build/libcjson.so
fi

for i in `ls -1 src/`  ; do
  if [[ "$i" != "include" ]]; then
    g++ -Isrc/include -std=c++20 -c src/$i -o build/${i%.cpp}.o
  fi
done

OBJS=""

for i in `ls -1 build/` ; do 
  if [[ "$i" != "karui" ]]; then
    OBJS="$OBJS build/$i"
  fi
done

g++ $OBJS -o build/karui -lcjson
rm $OBJS
