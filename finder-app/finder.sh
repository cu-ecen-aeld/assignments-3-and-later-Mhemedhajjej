#!/bin/sh
# Tester script for assignment 1
# Author: Mhemedhajjej

if [ "$#" -ne 2 ]; then
    echo "Error: Exactly 2 arguments are required. please provide the directory and the search string."
    exit 1
fi

filesdir=$1
searchstr=$2

if [ ! -d "$filesdir" ]; then
    echo "Error: Directory $filesdir does not exist."
    exit 1
fi

X=$(find "$filesdir" -type f | wc -l)  # Count the number of files containing the search string
Y=$(grep -r "$searchstr" "$filesdir" | wc -l)  # Count the number of matching lines

# output the results
echo "The number of files are $X and the number of matching lines are $Y"

#end of script
exit 0