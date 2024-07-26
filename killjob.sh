#!/bin/bash

# script to kill all jobs that you submitted
# usage:
# $ sh ./killjob.sh

# prepare for batch submission
user_name=`whoami`

for job in `squeue -u ${user_name} | grep -e "${user_name}" | awk  ' { print $1 } ' `; do
    echo "scancel ${job}"
    qdel ${job}
done

