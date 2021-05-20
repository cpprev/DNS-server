#!/bin/bash

# Testsuite script

# Colors
green='\033[1;32m'
blue='\033[0;34m'
purple='\033[0;35m'
cyan='\033[1;36m'
gray='\033[0;37m'
red='\033[1;31m'
reset='\033[0m'

# Verify that we are in the right folder (tests/)
dir="$(basename $PWD)"
if [ $dir != "tests" ]; then
    echo -e "$cyan""You need to be in the $red'~/tests/'$cyan" \
        "directory to launch the testsuite. $reset"
    exit 1
fi

# Logging
echo -e "$cyan""Current directory:" $PWD

# Utility functions

test_func_conf ()
{
    total_tests=$((total_tests + 1))

    expected=$1
    test_name=$2
    file=$3

    ./dns -t "${file}" &> /dev/null
    ret=$?

    # Error case: file is not valid
    if ([ "$expected" == "valid" ] && [ $ret -eq 0 ]) || ([ "$expected" == "invalid" ] && [ $ret -ne 0 ]); then
        echo -e "${green}[OK]""${cyan}""[$test_name] "
        tests_passed=$((tests_passed + 1))
    else
        echo -e "${red}[KO]""${cyan}""[$test_name]"
    fi
}

is_argument ()
{
    arg=$1
    nb=$2
    if [ $nb -eq 0 ]; then
        return 0
    fi
    temp=$3
    for elm in $arg; do
        if [ "$elm" == "$temp" ]; then
            return 0
        fi
    done
    return 1
}

# Counters initialization
total_tests=0
total_passed=0

####################################################

# Config files tests
if is_argument "$*" "$#" "config"; then
    . src/config.sh
fi

####################################################

# Logging
echo -e "$green\n[$tests_passed/$total_tests tests passed!]"

# Reset print color
echo -ne $reset

exit 0