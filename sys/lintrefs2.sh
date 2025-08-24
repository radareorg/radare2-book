#!/bin/sh
# Lint Markdown references by following links starting from a root file,
# while preventing infinite loops (cycles, self-links, symlink loops).
# POSIX sh compatible.

set -u

# repo root = script dir's parent
ROOT="$(cd "$(dirname "$0")/.." && pwd -P)"
START="${1:-src/SUMMARY.md}"

RV=0

# Track visited Markdown files to avoid cycles
VISITED="$(mktemp "${TMPDIR:-/tmp}/lintrefs.visited.XXXXXX")" || {
  echo "Failed to create temp file" >&2; exit 2; }
trap 'rm -f "$VISITED"' EXIT INT TERM

mark_visited() {
  # $1: absolute canonical path (or best effort) of a file
  printf '%s\n' "$1" >> "$VISITED"
}

is_visited() {
  # $1: absolute canonical path
  grep -Fxq "$1" "$VISITED" 2>/dev/null
}

# Canonicalize a path (best effort). If the path exists, we resolve with pwd -P.
# If it doesn't exist, we still return an absolute path (textual resolution).
canon_abs() {
  # $1: path (absolute or relative)
  case "$1" in
    /*) p="$1" ;;
    *)  p="$ROOT/$1" ;;
  esac

  if [ -e "$p" ]; then
    d=$(dirname -- "$p")
    b=$(basename -- "$p")
    if cd "$d" 2>/dev/null; then
      printf '%s/%s\n' "$(pwd -P)" "$b"
      cd "$OLDPWD" 2>/dev/null || :
      return 0
    fi
  fi

  # Fallback textual normalization (remove //, /./, and collapse a/b/..)
  old=
  while [ "$old" != "$p" ]; do
    old="$p"
    # remove // and /./
    p=$(printf '%s' "$p" | sed 's://*:/:g; s:/\./:/:g')
    # collapse /X/../
    p=$(printf '%s' "$p" | awk -F/ '
      {
        n=0
        for (i=1;i<=NF;i++) {
          if ($i=="" || $i==".") continue
          if ($i=="..") { if (n>0) n--; else stack[n++]=""; }
          else stack[n++]=$i
        }
        out=""
        for (i=0;i<n;i++) out=out "/" stack[i]
        if (out=="") out="/"
        print out
      }')
  done
  printf '%s\n' "$p"
}

# Resolve a link target relative to the *file* that referenced it.
# Returns absolute path (best effort). Echoes nothing on unsupported schemes.
resolve_target() {
  # $1 = absolute path of the referencing file
  # $2 = link as found in markdown (may include #fragment or ?query)
  refdir=$(dirname -- "$1")
  lnk="$2"

  case "$lnk" in
    ""|\#*|http://*|https://*|mailto:*|tel:*|data:*) return 1 ;;
  esac

  # strip fragment and query
  lnk="${lnk%%#*}"
  lnk="${lnk%%\?*}"

  case "$lnk" in
    /*) cand="$lnk" ;;
    *)  cand="$refdir/$lnk" ;;
  esac

  canon_abs "$cand"
}

# Extract links from a markdown file (both [text](link) and HTML href="link")
# Prints one link per line (raw, as in the file). No filtering here.
extract_links() {
  # $1: absolute path of file to parse
  f="$1"
  # 1) Markdown [text](link) occurrences (ignore images by allowing both but same rule for existence)
  # 2) HTML <a href="link"> and <img src="link"> as well
  # Note: heuristic, suficient per fer lint.
  sed -n 's/!\{0,1\}\[[^]]*\](([^)]*)/\n&\n/gp' "$f" 2>/dev/null | \
    sed -n 's/.*\[[^]]*\](([^)]*)).*/\1/p' 2>/dev/null
  sed -n 's/.*href[[:space:]]*=[[:space:]]*"\([^"]*\)".*/\1/p; s/.*href[[:space:]]*=[[:space:]]*'\''\([^'\'']*\)'\'' .*/\1/p' "$f" 2>/dev/null
  sed -n 's/.*src[[:space:]]*=[[:space:]]*"\([^"]*\)".*/\1/p; s/.*src[[:space:]]*=[[:space:]]*'\''\([^'\'']*\)'\'' .*/\1/p' "$f" 2>/dev/null
}

lint_file() {
  # $1: file path (absolute or relative); we will canonicalize and deduplicate
  abs="$(canon_abs "$1")"

  # Only process once
  if is_visited "$abs"; then
    return 0
  fi
  mark_visited "$abs"

  if [ ! -f "$abs" ]; then
    printf '\033[31mNot a file: %s\033[0m\n' "$abs" >&2
    RV=1
    return 0
  fi

  # Iterate over links
  extract_links "$abs" | while IFS= read -r raw; do
    [ -n "$raw" ] || continue

    # Skip anchors/schemes handled in resolve_target
    target="$(resolve_target "$abs" "$raw")" || continue

    if [ -e "$target" ]; then
      case "$target" in
        *.md|*.MD|*.Md|*.markdown|*.MARKDOWN)
          # Recurse into linked markdown, guarding with visited set.
          lint_file "$target"
          ;;
        *)
          : ;;
      esac
    else
      printf '\033[31mReferenced file %s not found. From %s\033[0m\n' "$raw" "$abs" >&2
      RV=$(( RV | 1 ))
    fi
  done
}

# Kick off
lint_file "$START"
exit "$RV"
