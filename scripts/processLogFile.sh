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
num_lines=$(wc -l < "$input") # Get the number of lines of the input file
# Delete previous files and create new
names="name_temp"
parties="pollerResultsFile"
rm -f ${names};touch ${names}
rm -f ${parties};touch ${parties}
total=0 # Total number of votes
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
    else    # Skip the person if he/she has already voted
        continue
    fi
    # Get the line in which the party appears in the file $parties (if it does appear)
    matching_lines=$(grep -E -n "^$party\s" "$parties")
    # Use read to split the string by spaces and : into an array
    IFS=': ' read -ra split <<< "$matching_lines"
    #  first arg->line found the match,second arg->party,third arg->votes counter of the party
    if [ -z "${split[*]}" ]; then # If no match found add it with vote 1
        echo "$party 1" >> "$parties"
    else # If party found just increment the number of votes of this party
        i="${split[0]}"
        counter="${split[2]}"
        ((counter++))
        new_str="$party $counter"
        sed -i "${i}s/.*/${new_str}/" "$parties" # Update the vote counter in the file
    fi
    ((total++))
done < "$input"
echo "TOTAL $total" >> "$parties"
sort -o $parties $parties
rm -f ${names};