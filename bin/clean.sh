#!/usr/bin/env bash
# ./clean.sh -t <target>


if [[ $* == *-a* ]] && [[ $ENVIRONMENT == *"local"* ]]; then
    declare -a DIRS=(build $PATH_DEPLOY dist)
else
    declare -a DIRS=(build dist)
fi

for DIR in ${DIRS[@]}; do
    if [[ -d $DIR ]]; then
        echo "🗑️  Cleaning up $DIR!"
        rm -rf $DIR
    fi
done