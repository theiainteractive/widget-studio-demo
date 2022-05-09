#!/usr/bin/env bash
# ./build.sh [-r]

# Set environment
ue4 setroot "$UE_ROOT"

# Clean/Create build directory
echo "📁  Creating build path: $PATH_BUILD"
mkdir -p "$PATH_BUILD"

# Move source to build directory, remove source control plugin, and package
cp -r src/$PACKAGE_NAME/. "$PATH_BUILD" || exit 1
rm -rf "$PATH_BUILD/Plugins/UE4GitPlugin" || exit 2

if [[ $PLATFORM_UE == "IOS" || $PLATFORM_UE == "Android" ]]; then
    sed -i "s|GameDefaultMap=.*|GameDefaultMap=/Game/WidgetStudioDemo/Maps/WidgetStudioDemo_Mobile.WidgetStudioDemo_Mobile|g" "$PATH_BUILD/Config/DefaultEngine.ini"
fi

# Build
pushd "$PATH_BUILD" || exit 3
    if [[ $* == *-b* ]]; then
        echo "📦  Building for $PLATFORM_UE..."
        BUILD_OPT="build"
    elif [[ $* == *-r* ]]; then
        echo "📦  Packaging for $PLATFORM_UE..."
        BUILD_OPT="package Shipping" 
    else
        echo "📦  Packaging for $PLATFORM_UE..."
        BUILD_OPT="package Development"
    fi
    ue4 $BUILD_OPT -platform=$PLATFORM_UE || exit 4
popd

# if build failed, exit
if [ ! -d "$PATH_BUILD/dist" ]; then
    echo "🚫  Build failed"
    exit 5
fi

# Move archive to the archive directory as ue4cli's `-archivedirectory` option is sometimes ignored
echo "📁  Renaming artifact folder..."
pushd "$PATH_BUILD/dist"
    for DIR in */; do
        mv "$DIR" "$PACKAGE_NAME"
        break
    done
popd

echo "📁  Moving artifact to $PATH_ARTIFACT..."
mkdir -p "$PATH_ARTIFACT"
if [[ $PLATFORM_UE_TARGET == "Android" ]]; then
    for artifact in "$PATH_BUILD/dist/*.apk"; do
      echo mv -- "$artifact" "$PATH_ARTIFACT/${artifact%.apk}_$VERSION_BUILD.apk" || exit 6
      break
    done
else
    # Add version to artifact
    echo "📁  Adding version $VERSION_BUILD to artifact..."
    echo "$VERSION_BUILD" > "$PATH_ARTIFACT/$PACKAGE_NAME/VERSION" || exit 7
    echo mv "$PATH_BUILD/dist/$PACKAGE_NAME" "$PATH_ARTIFACT" || exit 6
fi

echo "✔️   Build Successful"
