#!/bin/sh

RV=0
find_markdown_files() {
  (echo "$1" | grep "://") && return
  local D=`dirname "$1"`
  pushd "$D" > /dev/null
  local F=`basename "$1"`
  local file="$F"

  # Print the current file
  echo "$PWD/$F"

  # Extract referenced markdown files
  grep -o '\[.*\](.*\.md)' "$file" | sed -n 's/.*(\(.*\.md\)).*/\1/p' | while read -r ref; do
    (echo "$ref" | grep "://") && continue
    # Check if the referenced file exists
    if [ -f "$ref" ]; then
      # Call the function recursively
      find_markdown_files "$ref"
    else
      echo "\033[31mReferenced file '$ref' not found. From $PWD/$F\033[0m" > /dev/stderr
      RV=1
    fi
  done
  popd > /dev/null
}

# Check if a file is provided
F="$1"
[ -z "$1" ] && F=src/SUMMARY.md

# Start the recursive search with the provided file
find_markdown_files "$F"
exit $RV
