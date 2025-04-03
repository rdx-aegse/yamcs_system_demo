#!/bin/bash

#This script waits for N files matching pattern P in folder F when called using waitforfiles.sh F N P where F, N and P are optional

# Default values
N=1
pattern="*"
folder_path="."

# Parse command line arguments
if [ $# -ge 1 ]; then
    folder_path="$1"
fi

if [ $# -ge 2 ]; then
    N="$2"
fi

if [ $# -ge 3 ]; then
    pattern="$3"
fi

# Check if N is a number
if ! [[ "$N" =~ ^[0-9]+$ ]]; then
    echo "Error: First argument must be a number."
    echo "Usage: $0 [folder] [N] [pattern]"
    exit 1
fi

echo "Waiting for at least $N files matching pattern '$pattern' in '$folder_path'..."

while true; do
    file_count=$(find "$folder_path" -type f -name "$pattern" | wc -l)
    
    if [ "$file_count" -ge "$N" ]; then
        echo "Found $file_count files matching pattern '$pattern'. Resuming execution."
        break
    fi

    echo "Still waiting for $((N - file_count)) more files matching pattern '$pattern' in '$folder_path'..."
    
    sleep 1
done