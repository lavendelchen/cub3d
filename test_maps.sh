#!/bin/bash

ERROR_LOG="did_not_pass.log"
ERROR_DIR="./did_not_pass"

output=""

rm -rf $ERROR_DIR
rm -rf $ERROR_LOG

for valid_map in ./scene_descriptions/valid/*; do
	if ! output=$( (./cub3d "$valid_map") 2>&1); then
	  echo "$valid_map got flagged as invalid"
	  # Copy Valid Map that got flagged as invalid to $ERROR_DIR
    mkdir -p "$ERROR_DIR/valid"
	  cp "$valid_map" "$ERROR_DIR/valid/"
	  # Print Error Output to $ERROR_LOG
	  printf "%s\n%s\n" "$valid_map" "$output" >> $ERROR_LOG
	  # Print Error Output also to STDOUT
	  printf "%s\n%s\n" "./cub3d $valid_map" "$output"
	fi
done

for invalid_map in ./scene_descriptions/invalid/*; do
	if output=$( (./cub3d "$invalid_map") 2>&1); then
	  echo "$invalid_map got flagged as valid"
	  # Copy Invalid Map that got flagged as valid to $ERROR_DIR
    mkdir -p "$ERROR_DIR/invalid"
	  cp "$invalid_map" "$ERROR_DIR/invalid/"
	  # Print Error Output to $ERROR_LOG
	  printf "%s\n%s\n" "$invalid_map" "$output" >> $ERROR_LOG
	  # Print Error Output also to STDOUT
	  printf "%s\n%s\n" "./cub3d $invalid_map" "$output"
	fi
done