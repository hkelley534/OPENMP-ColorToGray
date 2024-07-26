#!/bin/sh

# Script to automatically generate submit script
# and automatically submit your jobs, based on
# the number of threads and matrix dimension
# Usage:
# $ sh ./SAXPY_mamba_submit.sh ${project_name} ${n} ${alpha} ${is_to_run}
#
# For example,
#    $ ./SAXPY_mamba_submit.sh SAXPYThread1 20 1.0 1

# prepare for batch submission
user_name=`whoami`

# prj_name is the .cpp name which includes the main function
prj_name=$1

# project path
prj_path=`pwd`

color_image=$2

gray_image=$3

image_type=$4

# is_to_run 1: yes (generating scripts and submit them to cluster)
#         0: no (only generating scripts)
is_to_run=$5

if [ ! -d "${prj_path}/submit" ] 
then
    mkdir ${prj_path}/submit
else
    rm -f ${prj_path}/submit/*
fi

cd ${prj_path}

rm -f ${prj_name}_${user_name}_qsub_*

# compile the code (this block may be changed according to a project)
echo "make clean"
make clean

rm -f slurm-*.out
rm -f ${prj_name}_${user_name}_qsub_*

echo "make all"
make all

for i in 1 2 4 8 16 32; do
    if [[ $i -le 16 ]]
    then
        nodes=1
        cores=$i
    else
        tmp=`expr $i + 15`
        cores=16
        nodes=1  #`expr ${tmp} / ${cores}`
    fi
    submitf=./submit/${prj_name}_${user_name}_srun_t${i}.sh
    inputf=./submit/${prj_name}_input_t${i}.log

    logf=./submit/${prj_name}_${user_name}_srun_t${i}.log

    echo "#! /bin/bash" > ${submitf}
    echo "#SBATCH --job-name=${prj_name}_${user_name}_srun_t${i}" >> ${submitf}
    echo "#SBATCH --partition=Centaurus"  >> ${submitf}
    echo "#SBATCH --nodes=${nodes}" >> ${submitf}
    echo "#SBATCH --ntasks-per-node=${cores}" >> ${submitf}
    echo "#SBATCH --time=00:05:00"  >> ${submitf}
    #echo "module load openmpi" >> ${submitf}
    echo "cd ${prj_path}" >> ${submitf}
    echo "ulimit -S -s 10240" >> ${submitf}
    echo "./${prj_name} ${color_image} t${i}_${gray_image} ${image_type} ${i}  > ${logf} " >> ${submitf} 

    if [[ ${is_to_run} -eq 1 ]]; then 
        echo "sbatch ${submitf}"
        sbatch ${submitf}
    fi 
  done

