#!/bin/bash

if [ "$#" -ne 1 ]; then
   printf "This program expects a command line argument.\n";
   printf "First, run your ngram program like this: ngram shakespeare.txt > ngram.out\n"
   printf "Then, run this program like this: ngramcheck.sh ngram.out\n";
   exit;
fi;

filename=$1
exec 4< $filename
IFS=''
prevcount=1000

#skip the first two line (the headers)
read -u 4 line
read -u 4 line

#check to make sure that the output lines are
#in descending order by count
regexp='^(.+)\s(.+),\s+([0-9]+)$'
while read -u 4 line      # : is the NULL command
do
   if [[ "$line" =~ $regexp ]]; then
      count="${BASH_REMATCH[3]}";
   else
      printf "Output not in form: <word1> <word2>, <count>\n";
      printf "Current line: %s\n" $line;
      exit;
   fi
   #printf "%s count: %s\n" "$line" "$count"; 
   if [[ $count -gt $prevcount ]]; then
      printf "Output file not in descending order by count\n";
      printf "Current line: %s\n" $line;
      exit;
   fi;
   prevcount=$count;
done 

#now sort the file and make sure it is identical
#to the correct output that has been sorted
sort $filename > "$filename.sorted"
diff "$filename.sorted" "/u/css/classes/3482/184/lab3/shakespeare.2grams.sorted" > diffs
if [ -s diffs ]; then
  printf "There are differences between your output and the correct output\n";
  cat diffs
  rm $filename.sorted
  rm diffs
else
  printf "Test passed\n";
fi
