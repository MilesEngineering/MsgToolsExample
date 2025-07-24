#!/bin/bash
# Copyright Miles Engineering, Inc.
# This should only need to happen once per dev machine.
# Install a bunch of OS and python dependencies, for several OSes.
# For other OSes, this script will need to be updated.

. ./embedded/ucplatform/mk/install_prerequisites.sh

# exit on any error
set -e
# exit on errors in pipes, too
set -o pipefail
# treat use of unset variables as an error and exit
#set -u
# set command printouts to show line numbers, so it's clear what step failed
PS4='### ${LINENO}> '
# print all commands to stderr
set -x

if [[ $OS_DESCRIPTION == *"Ubuntu"* ]]; then
# Nothing special for ubuntu
fi

exit 0 # Success!
