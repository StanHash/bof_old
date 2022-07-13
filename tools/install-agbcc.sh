#!/bin/bash

here=$(dirname "$(readlink -f "$0")")/..
temp=$(mktemp -d)

git clone https://github.com/pret/agbcc.git $temp

cd $temp
./build.sh
./install.sh $here

rm -fr $temp
