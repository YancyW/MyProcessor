#!/bin/bash


#(make sure the right shell will be used)
#$ -S /bin/bash
#
#execute the job from the current directory and not relative to your home directory
#$ -cwd

# sld5 for bird on NAF2, and sld6 on NAF1
#$ -l arch=amd64
#$ -l os=sld6

#$ -l h_rt=2:59:00
#$ -l h_vmem=4G

#set the email address
##$ -M yan.wang@desy.de
#
#send email when job aborts
#$ -m a

###==============================================================


 echo "############################################################################"
 echo "                             System information"
 echo "                             =================="

 echo "Host:"
 hostname -f

 echo "CPU(s):"
 cat /proc/cpuinfo | grep "model name" | cut -b 14-

 echo "RAM:"
 cat /proc/meminfo | grep "MemTotal" | cut -b 10-

 echo "Swap:"
 cat /proc/meminfo | grep "SwapTotal" | cut -b 11-

 echo "############################################################################"
 echo "Group:      ${GROUP}"

 echo "Hosttype:   ${HOSTTYPE}"

 echo "JobID:      ${JOB_ID}"

 echo "JobName:    ${JOB_NAME}"

 echo "Job_Script: ${JOB_SCRIPT}"

 echo "User:      ${LOGNAME}"

 echo "Queue:      ${QUEUE}"

 echo "Shell:      ${SHELL}"

 echo "TMP:        ${TMP}"

 echo "TMPDIR:     ${TMPDIR}"

 echo "User:       ${USER}"

 echo "Working_DIR:${PWD}"

 echo "############################################################################"
 echo
 echo "############################################################################"
 echo
 source /cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/init_ilcsoft.sh


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

name=${1}


root -l -q -b result.C"(${name})"
