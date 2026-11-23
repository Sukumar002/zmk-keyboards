#!/usr/bin/env bash
set -euo pipefail

usage() {
  echo "Usage: $0 --input DIR --output DIR --manifest FILE" >&2
  exit 2
}

input=""
output=""
manifest=""

while [[ $# -gt 0 ]]; do
  case "$1" in
    --input) input="${2:-}"; shift 2 ;;
    --output) output="${2:-}"; shift 2 ;;
    --manifest) manifest="${2:-}"; shift 2 ;;
    *) usage ;;
  esac
done

[[ -n "$input" && -d "$input" ]] || { echo "Input directory not found: $input" >&2; exit 1; }
[[ -n "$output" ]] || usage
[[ -n "$manifest" && -f "$manifest" ]] || { echo "Manifest not found: $manifest" >&2; exit 1; }

mkdir -p "$output"
shopt -s nullglob
declare -A targets=()
count=0

while IFS=$'\t' read -r source target || [[ -n "${source:-}" ]]; do
  [[ -z "${source//[[:space:]]/}" || "$source" == \\#* ]] && continue
  [[ -n "${target:-}" ]] || { echo "Invalid manifest row for $source" >&2; exit 1; }
  [[ -z "${targets[$target]+x}" ]] || { echo "Duplicate output filename: $target" >&2; exit 1; }
  targets[$target]=1

  matches=("$input/$source.uf2")
  if [[ ! -f "${matches[0]}" ]]; then
    echo "Missing build output: $source.uf2" >&2
    exit 1
  fi

  cp "${matches[0]}" "$output/$target"
  echo "Packaged: $source.uf2 -> $target"
  ((count += 1))
done < "$manifest"

(( count > 0 )) || { echo "Manifest produced no firmware files" >&2; exit 1; }
echo "Packaged $count firmware files."
