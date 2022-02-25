#!/usr/bin/env bash

pushd $PATH_ARTIFACT || exit
    if [[ -f "$SIG_PRECISE.zip" ]]; then
        echo "🗑️  Removing existing deployment artifact: $SIG_PRECISE.zip"
        rm "$SIG_PRECISE.zip"
    fi

    # Bundle archive into zip file
    [[ $PLATFORM_HOST == "windows" ]] && ZIP="7z a -tzip" || ZIP="zip -r"
    $ZIP "$SIG_PRECISE.zip" "$PACKAGE_NAME" || exit 9

    if [[ $ENVIRONMENT == *"local"* ]]; then
        CP="cp"
        mkdir -p $PATH_DEPLOY
    else
        CP="aws s3 cp"
        aws configure set default.s3.use_accelerate_endpoint true || exit 10
        aws configure set default.s3.multipart_threshold 300MB || exit 10
        REGION="--region us-west-2"
    fi

    $CP "./$SIG_PRECISE.zip" $PATH_DEPLOY_PRECISE $REGION || exit 11
    $CP $PATH_DEPLOY_PRECISE $PATH_DEPLOY_VERSION || exit 11
    $CP $PATH_DEPLOY_PRECISE $PATH_DEPLOY_LATEST || exit 11
popd