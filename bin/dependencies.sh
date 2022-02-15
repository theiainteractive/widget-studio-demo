#!/usr/bin/env bash
# ./dependencies.sh

aws configure set default.s3.use_accelerate_endpoint true
aws configure set default.s3.multipart_threshold 300MB

while read S3_URL NAME; do
    aws s3 cp "$S3_URL/$NAME-ue$VERSION_UE-$PLATFORM_UE-latest.zip" .
done <<< $(yq -r ".dependencies[] | [.s3, .name] | @tsv" config.yml)

