#!/bin/sh

cd $(dirname "$0")/..

posix_pushd() {
  if [ $# -eq 0 ]; then
    echo "Usage: pushd <directory>"
    return 1
  fi

  DIR=$1
  if [ ! -d "${DIR}" ]; then
    echo "pushd: ${DIR}: No such directory"
    return 1
  fi

  # Save the current directory on the stack
  DIR_STACK="$PWD $DIR_STACK"

  # Change to the new directory
  cd "${DIR}" || return 1

  # Print the new stack
  echo "$DIR_STACK"
}

# Function to pop a directory off the stack
posix_popd() {
  if [ -z "$DIR_STACK" ]; then
    echo "popd: directory stack empty"
    return 1
  fi

  # Extract the first directory from the stack
  DIR=$(echo "$DIR_STACK" | awk '{print $1}')
  DIR_STACK=$(echo "$DIR_STACK" | cut -d' ' -f2-)

  # Change to the directory
  cd "$DIR" || return 1

  # Print the new stack
  echo "$DIR_STACK"
}

RV=0
find_markdown_files() {
  (echo "$1" | grep "://") && return
  local D=`dirname "$1"`
  posix_pushd "$D" > /dev/null
  local F=`basename "$1"`
  local file="$F"

  echo "$PWD/$F"
  # Extract referenced markdown files
  grep -o '\[.*\](.*\.md)' "$file" | sed -n 's/.*(\(.*\.md\)).*/\1/p' | grep -v :// | while read -r ref; do
    if [ -f "$ref" ]; then
      find_markdown_files "$ref"
    else
      echo "\033[31mReferenced file '$ref' not found. From $PWD/$F\033[0m" > /dev/stderr
      RV=1
    fi
  done
  posix_popd > /dev/null
}

# Check if a file is provided
F="$1"
[ -z "$1" ] && F=src/SUMMARY.md

# Start the recursive search with the provided file
find_markdown_files "$F"
exit $RV
