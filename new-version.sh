#!/bin/bash

old_version=$1
new_version=$2

sed -i "s/$old_version/$new_version/g" ./Makefile
sed -i "s/$old_version/$new_version/g" ./README
sed -i "s/$old_version/$new_version/g" ./README.md
sed -i "s/$old_version/$new_version/g" ./VERSION
