#!/bin/sh
# Tester script for assignment 1
# Author: Mhemedhajjej

if [ "$#" -ne 2 ]; then
    echo "Error: Exactly 2 arguments are required. please provide the file and string to write."
    exit 1
fi

writefile=$1
writestr=$2

# Check if the directory exists, if not create it
writedir=$(dirname "$writefile")
if [ ! -d "$writedir" ]; then
    echo "Directory $writedir does not exist. Creating it."
    mkdir -p "$writedir"
fi

# Write the string to the file
echo "$writestr" > "$writefile"

#end of script
exit 0