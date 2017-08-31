#!/bin/bash

sudo apt-get install htop lrzsz tmux -y
sudo apt-get install groovy maven unzip -y

wget http://github.com/zhfkt/ComplexCi/releases/download/v0.1/networks.zip 
mv networks.zip ../data/
unzip ../data/networks.zip -d ../data/

cd ../Master_algorithm
mvn install
cd -

./make.sh

./regressionTest.sh -1
