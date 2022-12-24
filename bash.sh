#!/bin/bash
#echo "Username: $1"
#echo "Age: $2"
#echo "Full Name: $3"

welcome () { 
    echo "HELLO $2"
    return 55
}

welcome "MIKE" "BOOOKP"
echo $?
