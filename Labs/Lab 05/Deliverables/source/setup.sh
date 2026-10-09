#!/bin/bash

# * variables

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
PURPLE='\033[0;35m'
RESET='\033[0m' # No Color
readonly GREEN RED YELLOW CYAN PURPLE RESET

# * variables end

set -e
# clean build directory if exists
if [ -d "build" ]; then
    echo -e "${YELLOW}Cleaning existing build directory...${RESET}"
    rm -rf build
fi

# setup cmake preset; REMOVE after setting up preset
cmake --preset default-configure
echo -e "${PURPLE}In VS Code, open Command Palette and search for 'CMake: Select Build Preset' select 'Default' ${RESET}"
echo -e "${PURPLE}After succesfully setting up preset, syntax highlighting along with other features should be active. You can safely remove the preset section from "setup.sh" ${RESET}"

echo -e "${CYAN}Run ./run.sh for running the program ${RESET}"
