#!/bin/sh

# Script to automatically extract execution time
# usage:
# $ sh ./analysis_SAXPYThread.sh ${project_name} ${project_path} 
# for example,
#    $ ./analysis_SAXPYThread.sh SAXPYThread /users/qcheng1/3145/SAXPYThread

# prepare for batch submission
user_name=`whoami`

# prj_name is the .cpp name which includes the main function
prj_name=$1

# project path
prj_path=`pwd`
if [ ! -d "${prj_path}/submit" ] 
then
   echo "No running results"
   exit 999
fi

cd ${prj_path}

for i in 1 2 4 8 16 32; do
    logf=./submit/${prj_name}_${user_name}_srun_t${i}.log
    millisec=`cat ${logf} | grep -e "milliseconds" | awk -F'(' ' { print $2 } ' | awk -F' ' ' { print $1 } '`
    echo "numOfThreads=${i} millisecond=${millisec}" 
  done

