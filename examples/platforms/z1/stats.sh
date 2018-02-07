#!/bin/bash

PATTERN="text"
FILTER="."
MAP_FILE=../../../build/z1/examples/apps/cli/ot-cli-mtd.map

main() {
	rm stats.sections.tmp
	cp -f $MAP_FILE .

	sed -n -f stats.sed $MAP_FILE | sed -n "/$FILTER/p" | awk -f stats.awk

	#sed -n -f stats.sed $MAP_FILE | sed -n "/$FILTER/p" | awk 'BEGIN { sum = 0;} { tmp = strtonum($4); sum += tmp; } END { printf(" = 0x%x(%d)\n", sum, sum); }'

	echo "###"
	cat $MAP_FILE | sed -n "s/^\.\([a-zA-Z0-9_][a-zA-Z0-9_]*\)  *[x0-9a-f][x0-9a-f]*  *\([x0-9a-f][x0-9a-f]*\).*$/ \2\t\t\1/p"
}

parseopt() {
	while [ "$#" -gt 0 ]
	do
		case "$1" in
		-p|--pattern)
			PATTERN="$2"
			shift
			;;
		-f|--filter)
			FILTER="$2"
			shift
			;;
		-*)
			echo "Invalid option '$1'." >&2
			exit 1
			;;
		*)	;;
		esac
		shift
	done 
}

parseopt "$@"
main
