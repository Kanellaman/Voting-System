#! /bin/bash
#
if [ "$#" -ne 2 ]; then
  echo "Error: Two arguments are required."
  echo "Usage: ./create_input.sh politicalParties.txt numLines"
  exit 1
fi
input="$1"
if [ ! -f "$input" ]; then
  echo "Error: File does not exist or is not readable."
  exit 1
fi
if [ "$2" -le 0 ]; then
  echo "numLines should be > 0"
  exit 1
fi

NEW_FILE="input.txt"
rm -f ${NEW_FILE}
touch ${NEW_FILE}
for i in {1..20}; do
  random_name=""

  # Generate a random number within the desired range
  length=$(( RANDOM % (8) + 4 ))

  for (( i=0; i<$length; i++ )); do
    random_character=$(echo "$RANDOM" | md5sum | awk '{print substr($1, length($1)-1, 1)}')
    random_name="${random_name}${random_character}"
  done
  length=$((RANDOM % 11 +1))
  random_party=$(head -n "$length" "$input" | tail -n 1)
  echo "$random_name $random_party" >> ${NEW_FILE}
done