#!/usr/bin/env bash
# ./test.sh 

# Tell ue4cli to use the UE root directory for the current version
ue4 setroot "$UE_ROOT"

# function to get string between quotes from a line of code.
# arg1 = line to parse
get_name_between_quotes () {
  name_in_quotes=""
  local is_in_quotes=false
  local string=$1

  for (( i=0; i<${#string}; i++ )); do
    character="${string:$i:1}"
    if [[ $is_in_quotes == false ]] && [[ $character == "\"" ]]; then
      is_in_quotes=true     
    elif [[ $is_in_quotes == true ]] && [[ $character == "\"" ]]; then
      is_in_quotes=false
      break
    elif [[ $is_in_quotes == true ]] && [[ $character != "\"" ]]; then
      name_in_quotes+=$character
    fi
  done 
}

TEST_FLAGS="--withrhi"
TESTS=""

# find all .spec.cpp files
REGEX='BEGIN_DEFINE_SPEC{1}\s*\(.*\".*\".*'
for file in $(find src/$PACKAGE_NAME -name "*.spec.cpp"); do
  # add tests named in BEGIN_DEFINE_SPEC section of file
  while IFS= read -r line; do
    if [[ "$line" =~ $REGEX ]]; then
      get_name_between_quotes "$line"
      TESTS+="\"$name_in_quotes\" "
    fi
  done <"$file"
done

# add BP functional tests
TESTS+="\"Project.Functional Tests./$PACKAGE_NAME/Tests\" "

# Run ue4cli tests
pushd src/$PACKAGE_NAME
    ue4 test $TEST_FLAGS $TESTS
popd

# Check tests successful
retval=$?
if [ $retval -ne 0 ]; then
    echo "❗ ERROR: Automated tests failed" : Return code: "$retval
    exit $retval
fi