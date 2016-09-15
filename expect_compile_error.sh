#!/bin/bash

# This script is designed to test facilities implemented with
# e.g. static_cast to produce more useful error messages.  It executes
# a compilation that is expected to fail with a particular error
# message.  All of this activity takes place within a separate test
# subdirectory so that multiple tests using this script can be run in
# parallel
#
# NOTE: this script assumes that the CXX environment variable is set
# appropriately before it is called.

function clean_up
{
    cd ..
    rm -r ${TEST_DIR}
}

# check for existence of header substitution file
if [[ ! -f $1 ]]; then
    echo "header substitution file '$1' does not exist"
    exit 1
fi

# argument is the name of a file that will be used to generate a
# header file for the compile source
HEADER="$1"

# generate the base string for various names
BASE=`echo $1 | sed 's/\.compile_error//'`

# generate related name of expected error text file
ERROR_TEXT_FILE="$BASE.error"

# generate name of test directory
export TEST_DIR=".`basename $BASE`"

if [[ -d ${TEST_DIR} ]]; then
    rm -r ${TEST_DIR}
fi

# create the test directory
mkdir ${TEST_DIR}

# copy base source file
cp test/fail_compile.cpp ${TEST_DIR}

# substitute header for this particular case
cp ${HEADER} ${TEST_DIR}/bad_decl.h

# check for existence of expected error text file
if [[ ! -f "${ERROR_TEXT_FILE}" ]]; then
    echo "expected error text file '${ERROR_TEXT_FILE}' does not exist"
    clean_up
    exit 1
fi
ERR_TEXT=`cat ${ERROR_TEXT_FILE}`

# copy error text file and reset filename value
cp ${ERROR_TEXT_FILE} ${TEST_DIR}
ERROR_TEXT_FILE=`basename ${ERROR_TEXT_FILE}`

cd ${TEST_DIR}

# compile
eval "${CXX} -I. -std=c++14 -Wno-deprecated -c fail_compile.cpp > ./fail_compile.out 2>&1";

# check compile result
if [ $? -eq 0 ]; then
    echo "compilation unexpectedly succeeded"
    clean_up
    exit 1
fi

if [[ -f ./fail_compile.out ]]; then
    echo "fail_compile.out present as expected"
else
    echo "WARN: fail_compile.out missing after compile"
fi

echo "compilation failed as expected"
# dump compiler output to .log file
echo
echo "********** start of compiler output **********"
cat ./fail_compile.out
echo "********** end of compiler output **********"
echo

# check for presence of expected error text
grep -q "`cat ${ERROR_TEXT_FILE}`" ./fail_compile.out

if [ $? -ne 0 ]; then
    echo "compilation failed but unable to find expected text '`cat ${ERROR_TEXT_FILE}`' in output"
    clean_up
    exit 1
fi

echo "found error text '`cat ${ERROR_TEXT_FILE}`' in compiler output as expected"
clean_up
exit 0
