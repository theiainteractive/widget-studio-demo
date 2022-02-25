#!/usr/bin/env bash
# ./run.sh -t <target>  -v <version> -p <platform>

while getopts :t:v:p: flag; do
    case "${flag}" in
        t) target=${OPTARG};;
        v) version=${OPTARG};;
        p) platform=${OPTARG};;
    esac
done

# Create artifact path
ARTIFACT_PATH="${target}/${version}/${platform}"

if [ ! -d "${ARTIFACT_PATH}" ]; then
    echo "🔥  Artifact path not found, aborting."
    exit 1
fi

