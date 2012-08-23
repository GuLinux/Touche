#!/bin/bash

HEADER_BEGIN="/***********************************************************************"
HEADER_END="***********************************************************************/"


function applyToFile() {
	file="$1"
	tempFile="/tmp/$( basename $0).tmp.cxx"
	echo "Applying license header to $file"
	LICENSE_START_AT="$( cat "$file" | grep -n -F "$HEADER_END" | cut -d: -f1 )"
	if test "x$LICENSE_START_AT" == "x"; then LICENSE_START_AT="-2"; fi
	cat "$( dirname $0)/LicenseHeader.txt" > "$tempFile"
	echo -e "\n" >> "$tempFile"
	cat "$file" | tail -n +$(($LICENSE_START_AT+2)) >> "$tempFile"
	cat "$tempFile" > "$file"
}

function findAll() {
	find -type f \( -name "*.cpp" -or -name "*.h" \) -and -not -name "moc_*.cpp" -exec "$0" applyTo {} \;
}


case "$1" in
	"applyTo")
	applyToFile "$2"
	;;
	*)
	findAll
	;;
esac
