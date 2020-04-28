#!/usr/bin/env bash
## Copyright 2018 Smx
## Copyright 2020 Mizux
set -euxo pipefail
shopt -s nocasematch

gamedir="$1"
if [ -z "$gamedir" ]; then
	echo "Usage: $0 [gamedir]"
	exit 0
fi

if [ -f "$gamedir/RUNTIME.KIX" ]; then
	datadir="$gamedir"
elif [ -f "$gamedir/DATA/RUNTIME.KIX" ]; then
	datadir="$gamedir/DATA"
else
	echo "Wrong game folder specified!"
	exit 1
fi

scriptdir="$(readlink -f "$(dirname "$0")")"
# Unpacker must be in the same location than this script
command -v "${scriptdir}/kunpackerbin"

for kix in "$datadir"/*.KIX; do
	# if no files found do nothing
	[[ -e "$kix" ]] || continue

	name=$(basename "${kix%.*}")
	if [ -d "$datadir/$name" ]; then
		echo "Skipping ${name}.KIX extraction, already extracted"
		continue
	fi

	kbf="${datadir}/${name}.KBF"
	if [ ! -f "$kbf" ]; then
		echo "Skipping ${name}.KIX extraction, missing ${name}.KBF file"
		continue
	fi

	echo "Extracting $name..."
	"${scriptdir}/kunpackerbin" "$kix" "$kbf"
done
