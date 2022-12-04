#!/bin/bash

conan install .. -pr=default

#If errors then exit
if [ "$?" != "0" ]; then
    exit -1
fi