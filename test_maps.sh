#!/bin/bash


BOLD="\e[1m"
RED="\e[41m"
GREEN="\e[32m"
RESET="\e[0m"

ERROR_LOG="did_not_pass.log"
ERROR_DIR="./did_not_pass"

output=""

rm -rf $ERROR_DIR
mkdir $ERROR_DIR
rm -rf $ERROR_LOG
touch $ERROR_LOG

for valid_map in ./scene_descriptions/valid/*; do
	if ! output=$( (./cub3d "$valid_map") 2>&1); then
    mkdir -p "$ERROR_DIR/valid"
	  echo "$valid_map got flagged as invalid"
	  cp "$valid_map" "$ERROR_DIR/valid/"
	  printf "%s\n%s\n" "$valid_map" "$output" >> $ERROR_LOG
	fi
done

for invalid_map in ./scene_descriptions/invalid/*; do
	if output=$( (./cub3d "$invalid_map") 2>&1); then
	  echo "$invalid_map got flagged as valid"
    mkdir -p "$ERROR_DIR/invalid"
	  cp "$invalid_map" "$ERROR_DIR/invalid/"
	  printf "%s\n%s\n" "$invalid_map" "$output" >> $ERROR_LOG
	fi
done