#!/bin/env sh

vers="$(git describe --abbrev=4 --match='v[0-9]*' HEAD 2>/dev/null)"

if [ -z "$vers" ]; then
	vers="0.0"
fi

major="$(printf "%s" "${vers}" | cut -d. -f1)"
minor="$(printf "%s" "${vers}" | cut -d. -f1)"

commit="$(git rev-parse --short HEAD)"

full_vers="${vers}-${commit}"
if [ -n "$(git status)" ]; then
	full_vers="${full_vers}-dirty"
fi

sed -e "s/@MAJOR@/${major}/g" \
	-e "s/@MINOR@/${minor}/g" \
	-e "s/@COMMIT@/${commit}/g"\
	-e "s/@FULLVERSION@/${full_vers}/g" \
	-e "s/@DATE@/$(date)/g" \
	"$1" > "$2"
