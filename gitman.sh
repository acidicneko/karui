#!/bin/bash
install() {
  ./configure.sh
  strip --strip-debug build/karui
  mv build/karui "$GITMAN_BIN"/karui
}

uninstall() {
  rm "$GITMAN_BIN"/karui
}

update(){
  install
}

if [ $1 = "install" ] ; then 
	install
elif [ $1 = "uninstall" ] ; then
	uninstall
elif [ $1 = "update" ] ; then
	update
else
	echo unknown option
	exit 1
fi
