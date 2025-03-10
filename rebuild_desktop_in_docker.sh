#!/bin/bash
set -e
git pull
rm result -rf
trap 'sudo apt remove calculator-task -y' ERR
docker build --target=qt_from_repo . -t calculator-task-build
idTempContainer=$(docker create calculator-task-build)
docker cp "$idTempContainer":/result .
docker rm "$idTempContainer"
sudo dpkg -i result/*.deb