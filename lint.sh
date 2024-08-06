#!/bin/sh

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
  DIR_STACK=("$PWD" "${DIR_STACK[@]}")

  # Change to the new directory
  cd "${DIR}" || return 1

  # Print the new stack
  echo "${DIR_STACK[@]}"
}

# Function to pop a directory off the stack
posix_popd() {
  if [ ${#DIR_STACK[@]} -eq 0 ]; then
    echo "popd: directory stack empty"
    return 1
  fi

  # Change to the top directory on the stack
  cd "${DIR_STACK[0]}" || return 1

  # Remove the top directory from the stack
  DIR_STACK=("${DIR_STACK[@]:1}")

  # Print the new stack
  echo "${DIR_STACK[@]}"
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
