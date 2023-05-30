#! /bin/bash
#
if [ "$#" -ne 1 ]; then
  echo "Error: One argument is required."
  echo "Usage: ./tallyVotes.sh tallyResultsFile"
  exit 1
fi
input="$1"
if [ ! -f "$input" ]; then
  echo "Error: File does not exist or is not readable."
  exit 1
fi
names="name_temp"
parties="tallyResultsFile"
rm -f ${names}
touch ${names}
rm -f ${parties}
touch ${parties}
num_lines=$(wc -l < "$input")
# Traverse every line of the input file (Even the last line)
for ((line_number=1; line_number<num_lines+2; line_number++)); do
    # Read the line with proper format
    IFS=' ' read -r name surname party
    # Check for empty line
    if [ -z "$name" ] && [ -z "$surname" ] && [ -z "$party" ]; then
        continue
    fi
    full_name="$name $surname"
    matching_lines=$(grep -x "$full_name" "$names") # Explicitly find the same name line by line
    if [ -z "$matching_lines" ]; then
        echo $full_name >> $names
    else
        continue
    fi
    
    flag=0 # A flag-variable to determine if the party already has been read before
    i=1
    while IFS=' ' read -r line counter; do
       if [ "$line" == "$party" ]; then
           flag=1 # If party is found dont insert it again
           break
        fi
    ((i++)) # Store the number of the line in case we need this to update the vote count of a party
    done < "$parties"
    # If party not found insert it with a vote of 1
    if [ "$flag" -eq 0 ]; then
       echo "$party 1" >> $parties
    else # If party found just increment the number of votes of this party 
        ((counter++))
        new_str="$party $counter"
        sed -i "${i}s/.*/${new_str}/" "$parties" # Update the vote counter in the file
    fi
done < "$input"
sort -o $parties $parties
rm -f ${names};