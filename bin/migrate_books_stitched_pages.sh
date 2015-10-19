#!/bin/bash

task="Migrate stitched book pages by book identifier"

drush scr script.php --task="List all books" > books.txt

cat books.txt | while read line
   do
     echo "Migrate stitched book pages from book ${line}"
     drush scr script.php --task="${task}" --identifier="${line}" ;
done

echo "Task ${task} done."

exit 0
