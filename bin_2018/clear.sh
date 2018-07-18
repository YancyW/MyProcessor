#!/bin/bash

if [ $#==1 ] ; then 
   echo "do you really want to delete all things?  if yes please use ./clear.sh y"	
elif [ $#==2 && $2=="y" ] ; then 
	echo "Begin to clear..."
else
	exit
fi
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"
home_folder=${DIR}/.. 

mass_profile=(10 30 50 70 90 115 125)
mass_length=${#mass_profile[@]}


for (( j=0; j<$(( $mass_length)); j++ )) 
do
	echo "for mass=$mass ..."
	mass=${mass_profile[j]}
	if [ -d ${home_folder}/results_${mass}  ] ; then
		echo "Already have  results_${mass}, clear them!"
		if [ ! "$(ls -A ${home_folder}/results_${mass})" ]; then
			echo "results_${mass} is empty!"
		else
	 		mv ${home_folder}/results_${mass}/*  ~/tmp/
		fi
	else 
		echo "no results_${mass} folder"
	fi
	if [ -d ${home_folder}/plot_${mass}  ] ; then
		echo "Already have  plot_${mass}, clear them!"
		if [ ! "$(ls -A ${home_folder}/plot_${mass})" ]; then
			echo "plot_${mass} is empty!"
		else
			mv ${home_folder}/plot_${mass}/* ~/tmp/
		fi
	else 
		echo "no plot_${mass} folder"
	fi
	if [ -d ${home_folder}/data_${mass}  ] ; then
		echo "Already have  data_${mass}, clear them!"
		if [ ! "$(ls -A ${home_folder}/data_${mass})" ]; then
			echo "data_${mass} is empty!"
		else
	 		mv ${home_folder}/data_${mass}/* ~/tmp/
		fi
	else 
		echo "no data_${mass} folder"
	fi
done

echo "clear all, you still can find them in ~/tmp folder!"
