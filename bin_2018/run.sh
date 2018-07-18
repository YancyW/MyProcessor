#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"
cd ${DIR}/../xml_isr
if [[ -f $1 ]] ; then
	Marlin $1 
fi
