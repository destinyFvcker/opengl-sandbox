#!/bin/bash
set -euo pipefail

PROJECT_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

if [ "$#" -lt 1 ]; then
  echo "Usage: $0 <executable-name|path> [args ...]" >&2
  echo "Example: $0 hello_triangle" >&2
  exit 1
fi

TARGET="$1"
shift

if [[ "$TARGET" = /* ]]; then
  EXE_PATH="$TARGET"
elif [[ "$TARGET" == */* ]]; then
  if [ -f "$PROJECT_ROOT/$TARGET" ]; then
    EXE_PATH="$PROJECT_ROOT/$TARGET"
  else
    EXE_PATH="$PROJECT_ROOT/cpp/build/$TARGET"
  fi
else
  MATCHES=()
  while IFS= read -r path; do
    MATCHES+=("$path")
  done < <(find "$PROJECT_ROOT/cpp/build" -type f -name "$TARGET" -perm -111 2>/dev/null)

  if [ "${#MATCHES[@]}" -eq 1 ]; then
    EXE_PATH="${MATCHES[0]}"
  elif [ "${#MATCHES[@]}" -gt 1 ]; then
    echo "Multiple executables named '$TARGET' found:" >&2
    for path in "${MATCHES[@]}"; do
      rel="${path#$PROJECT_ROOT/}"
      echo "  - $rel" >&2
    done
    echo "Please run with a relative path, e.g. ./run-cpp.sh cpp/build/<subdir>/$TARGET" >&2
    exit 1
  else
    EXE_PATH="$PROJECT_ROOT/cpp/build/$TARGET"
  fi
fi

if [ ! -f "$EXE_PATH" ]; then
  echo "Executable not found: $EXE_PATH" >&2
  echo "Tip: run ./build-cpp.sh first." >&2
  exit 1
fi

if [ ! -x "$EXE_PATH" ]; then
  echo "File is not executable: $EXE_PATH" >&2
  exit 1
fi

cd "$PROJECT_ROOT"
exec "$EXE_PATH" "$@"
