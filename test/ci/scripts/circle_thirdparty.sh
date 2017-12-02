#!/bin/bash

invalidate=false
cache_dir="$HOME/.local/thirdparty"
thirdparty_builder_sha1_path="$HOME/.local/thirdparty_builder_sha1"
thirdparty_sums_path="$HOME/.local/thirdparty_sums.md5"
thirdparty_yml_path="thirdparty.yml"

cat $thirdparty_sums_path
md5sum -c $thirdparty_sums_path
if [[ ! $? -eq 0 ]]; then
    echo "Either our script has changed or thirdparty.yml has changed"
    invalidate=true
    md5sum $thirdparty_yml_path $0 > $thirdparty_sums_path
fi

# Invalidate from ThirdParty-Builder
thirdparty_builder_remote_sha1=$(
    curl -sH 'User-Agent: Lugdunum3D/ThirdParty-Builder build script' \
    'https://api.github.com/repos/Lugdunum3D/ThirdParty-Builder/commits?page=1&per_page=1' \
    | python -c 'import sys, json; print(json.load(sys.stdin)[0]["sha"][:7])'
)

# Line below might seem a little hacky. If we don't have the file we want an empty
# string. But we also want a 0 return code, so we pipe to cat again to discard the error
cached_sha1=$(cat $thirdparty_builder_sha1_path 2>/dev/null | cat)

echo "cached_sha1: '$cached_sha1', thirdparty_builder_remote_sha1: '$thirdparty_builder_remote_sha1'"
if [[ $cached_sha1 != $thirdparty_builder_remote_sha1 ]]; then
    echo "ThirdParty-Builder differs, invalidating cache"
    echo "$thirdparty_builder_remote_sha1" > "$thirdparty_builder_sha1_path"
    invalidate=true
else
    echo "ThirdParty-Builder sha1 checks cache is valid"
fi

# Exit if a command fails
set -e

if [[ ! -d "$cache_dir" || "$invalidate" = true ]]; then
    rm -rf "$cache_dir"
    mkdir "$cache_dir"

    # Clone the repository
    git clone git@github.com:/Lugdunum3D/Thirdparty-Builder.git ~/Thirdparty-Builder
    cd ~/Thirdparty-Builder

    # Setup the python environment
    virtualenv venv
    source ./venv/bin/activate
    pip install -r requirements.txt

    # Build thirdparty
    python ./build.py -vvv --path $cache_dir -z linux.zip ~/LugBench/thirdparty.yml

    echo "Done building!"

    if [[ $CIRCLE_NODE_INDEX -eq 0 ]]; then
        thirdparty_short_sha1=$(git -C ~/LugBench log -n 1 --pretty=format:%h -- thirdparty.yml)

        echo "Uploading with sha1 $thirdparty_short_sha1..."

        echo -n $(md5sum linux.zip | cut -c -32) > linux.md5
        curl --user "upload:${UPLOAD_PASSWORD}" -T linux.md5 "https://thirdparty-dl.lugbench.eu/${thirdparty_short_sha1}/linux.md5"
        curl --user "upload:${UPLOAD_PASSWORD}" -T linux.zip "https://thirdparty-dl.lugbench.eu/${thirdparty_short_sha1}/linux.zip"

        echo "Done uploading! Zip available at https://thirdparty-dl.lugbench.eu/${thirdparty_short_sha1}/linux.zip"
    else
        echo "Skipping uploading (CIRCLE_NODE_INDEX is $CIRCLE_NODE_INDEX)"
    fi
fi
