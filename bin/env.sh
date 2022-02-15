#!/usr/bin/env bash

## Misc ##
export ENVIRONMENT=$(cat .env | sed -n 1p)
[[ ! -z $VERSION_UE_OVERRIDE ]] && export VERSION_UE=$VERSION_UE_OVERRIDE || export VERSION_UE=$(cat .env | sed -n 2p)
export VERSION_BUILD=$(cat .env | sed -n 3p)
PLATFORM_UE_TARGET=$(cat .env | sed -n 4p)
export PACKAGE_NAME=$(yq -r .name config.yml)


## Platform ##
if [[ $(uname) == *"MINGW"* ]]; then
    ENGINE_DEFAULT="C:\Program Files (x86)\Epic Games\UE_$VERSION_UE"
    LAUNCHER_INSTALLED_PATH="/c/ProgramData/Epic/UnrealEngineLauncher/LauncherInstalled.dat"
    export PLATFORM_UE_HOST="Win64"
    export PLATFORM_HOST="windows"
    export PLATFORM_PYTHON="win32"
elif [[ $(uname) == *"Darwin"* ]]; then
    ENGINE_DEFAULT="/Users/Shared/Epic Games/UE_$VERSION_UE"
    export PLATFORM_UE_HOST="Mac"
    export PLATFORM_HOST="macos"
    export PLATFORM_PYTHON="darwin"
else
    ENGINE_DEFAULT="$HOME/EpicGames/Engine/UE_$VERSION_UE"
    export PLATFORM_UE_HOST="Linux"
    export PLATFORM_HOST="ubuntu"
    export PLATFORM_PYTHON="linux2"
fi
[[ ! -z $PLATFORM_UE_TARGET ]] && export PLATFORM_UE=$PLATFORM_UE_TARGET || export PLATFORM_UE=$PLATFORM_UE_HOST


## Find Engine Path ##
if [[ -f $LAUNCHER_INSTALLED_PATH ]]; then 
    while IFS=$'\t' read -r InstallLocation ArtifactId; do
        ENGINE_VERSION=$(echo $ArtifactId | xargs)
        ENGINE_LOCATION=$(echo $InstallLocation | xargs)
        if [[ $ENGINE_VERSION == *$VERSION_UE ]]; then
            export UE_ROOT=$ENGINE_LOCATION
            break
        fi
    done <<< $(jq -r '.InstallationList[]|[.InstallLocation, .ArtifactId] | @tsv' $LAUNCHER_INSTALLED_PATH)
else
    export UE_ROOT=$ENGINE_DEFAULT
fi


## Git ##
export BRANCH=$(git rev-parse --abbrev-ref HEAD | tr /\'\" _)
export COMMIT=$(git rev-parse --short=8 HEAD)


## Signatures ##
SIG="$PACKAGE_NAME-ue$VERSION_UE-$PLATFORM_UE"          # Build name
export SIG_VERSION="$SIG-$VERSION_BUILD"                # Build name + version
export SIG_PRECISE="$SIG_VERSION-$COMMIT"               # Build name + version + commit
export SIG_LATEST="$SIG-latest"                         # Build name + 'latest'


## Paths ##
export PATH_ARTIFACT="$(pwd)/dist/$VERSION_UE/$PLATFORM_UE"
export PATH_ARTIFACT_RELATIVE="dist/$VERSION_UE/$PLATFORM_UE"
export PATH_BUILD="$(pwd)/build/$VERSION_UE/$PLATFORM_UE"
export PATH_DEPLOY=$(yq -r .envs.\"$ENVIRONMENT\".deploy config.yml)
export PATH_DEPLOY_VERSION="$PATH_DEPLOY/$SIG_VERSION.zip"
export PATH_DEPLOY_PRECISE="$PATH_DEPLOY/$SIG_PRECISE.zip"
export PATH_DEPLOY_LATEST="$PATH_DEPLOY/$SIG_LATEST.zip"


if [[ $* == *"-s"* ]]; then
    ## Echo Environment ##
    echo "🏛️   Platform"
    echo "    💬  UE_ROOT:                $UE_ROOT"
    echo "    💬  PLATFORM_UE:            $PLATFORM_UE"
    echo "    💬  PLATFORM_UE_HOST:       $PLATFORM_UE_HOST"
    echo "    💬  PLATFORM_PYTHON:        $PLATFORM_PYTHON"
    echo "    💬  PLATFORM_HOST:          $PLATFORM_HOST"
    echo "📚  Miscellaneous"
    echo "    💬  ENVIRONMENT:            $ENVIRONMENT"
    echo "    💬  VERSION_UE:             $VERSION_UE"
    echo "    💬  VERSION_BUILD:          $VERSION_BUILD"
    echo "    💬  PACKAGE_NAME:           $PACKAGE_NAME"
    echo "👽  Git"
    echo "    💬  BRANCH:                 $BRANCH"
    echo "    💬  COMMIT:                 $COMMIT"
    echo "🔑  Signatures"
    echo "    💬  SIG:                    $SIG"
    echo "    💬  SIG_VERSION:            $SIG_VERSION"
    echo "    💬  SIG_PRECISE:            $SIG_PRECISE"
    echo "    💬  SIG_LATEST:             $SIG_LATEST"
    echo "🚋  Paths"
    echo "    💬  PATH_ARTIFACT:          $PATH_ARTIFACT"
    echo "    💬  PATH_ARTIFACT_RELATIVE: $PATH_ARTIFACT_RELATIVE"
    echo "    💬  PATH_BUILD:             $PATH_BUILD"
    echo "    💬  PATH_DEPLOY:            $PATH_DEPLOY"
    echo "    💬  PATH_DEPLOY_VERSION:    $PATH_DEPLOY_VERSION"
    echo "    💬  PATH_DEPLOY_PRECISE:    $PATH_DEPLOY_PRECISE"
    echo "    💬  PATH_DEPLOY_LATEST:     $PATH_DEPLOY_LATEST"
fi

if [[ $* == *"-i"* ]]; then
    ## Inject Environment Into Github Actions ##
    echo "💉  Injecting Environment Into Github Actions"
    echo "::set-output name=UE_ROOT::$UE_ROOT"
    echo "::set-output name=PLATFORM_UE::$PLATFORM_UE"
    echo "::set-output name=PLATFORM_UE_HOST::$PLATFORM_UE_HOST"
    echo "::set-output name=PLATFORM_PYTHON::$PLATFORM_PYTHON"
    echo "::set-output name=PLATFORM_HOST::$PLATFORM_HOST"
    echo "::set-output name=ENVIRONMENT::$ENVIRONMENT"
    echo "::set-output name=VERSION_BUILD::$VERSION_BUILD"
    echo "::set-output name=PACKAGE_NAME::$PACKAGE_NAME"
    echo "::set-output name=BRANCH::$BRANCH"
    echo "::set-output name=COMMIT::$COMMIT"
    echo "::set-output name=SIG::$SIG"
    echo "::set-output name=SIG_VERSION::$SIG_VERSION"
    echo "::set-output name=SIG_PRECISE::$SIG_PRECISE"
    echo "::set-output name=SIG_LATEST::$SIG_LATEST"
    echo "::set-output name=PATH_ARTIFACT::$PATH_ARTIFACT"
    echo "::set-output name=PATH_ARTIFACT_RELATIVE::$PATH_ARTIFACT_RELATIVE"
    echo "::set-output name=PATH_BUILD::$PATH_BUILD"
    echo "::set-output name=PATH_DEPLOY::$PATH_DEPLOY"
    echo "::set-output name=PATH_DEPLOY_VERSION::$PATH_DEPLOY_VERSION"
    echo "::set-output name=PATH_DEPLOY_PRECISE::$PATH_DEPLOY_PRECISE"
    echo "::set-output name=PATH_DEPLOY_LATEST::$PATH_DEPLOY_LATEST"
fi
