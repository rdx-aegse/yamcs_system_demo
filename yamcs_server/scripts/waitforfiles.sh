#!/bin/bash

if [ $# -eq 0 ]; then
    echo "Usage: $0 <folder_path>"
    exit 1
fi

folder_path="$1"

if [ ! -d "$folder_path" ]; then
    echo "Error: '$folder_path' is not a valid directory."
    exit 1
fi

echo "Waiting for files in folder '$folder_path'..."

while [ -z "$(ls -A "$folder_path")" ]; do
    sleep 1
done

echo "Folder is no longer empty. Resuming execution"
