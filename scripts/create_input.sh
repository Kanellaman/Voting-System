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

NEW_FILE="../inputFile"
rm -f ${NEW_FILE}
touch ${NEW_FILE}
num_lines=$(wc -l < $input)
for (( j=0; j<$2; j++ )); do
  
  random_name=$(echo "$((RANDOM % 26 + 65))" | awk '{ printf "%c", $0 }')
  # Generate a random number within the desired range
  length=$(( RANDOM % (8) + 4 ))
  for (( i=0; i<$length-1; i++ )); do
    random_character=$(echo "$((RANDOM % 26 + 97))" | awk '{ printf "%c", $0 }')
    random_name="${random_name}${random_character}"
  done

  random_surname=$(echo "$((RANDOM % 26 + 65))" | awk '{ printf "%c", $0 }')
  length=$(( RANDOM % (8) + 4 ))
  for (( i=0; i<$length-1; i++ )); do
    random_character=$(echo "$((RANDOM % 26 + 97))" | awk '{ printf "%c", $0 }')
    random_surname="${random_surname}${random_character}"
  done
  
  length=$((RANDOM % $num_lines +1))
  random_party=$(head -n "$length" "$input" | tail -n 1)
  echo "$random_name $random_surname $random_party" >> ${NEW_FILE}
done