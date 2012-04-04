#!/usr/bin/env bash
opt=0
for option in $*
do
	pos=`expr index "$option" =`
	argument=${option:1:pos-2}
	value=${option:pos}
	if [ $argument = "backend" ]
	then
		if  [ $value = "c" ]
		then	
		    if [ $opt = 0 ]
		    then
			./convert
		    elif [ $opt = 11 ]
		    then
			./cfg_scp_3addr | ./convert
		    elif [ $opt = 12 ]
		    then
			./cfg_dse_3addr | ./convert
		    fi
		elif [ $value = "cfg" ]
		then
		    if [ $opt = 0 ]
		    then
			./cfg_cfg
		    elif [ $opt = 11 ]
		    then
			./cfg_scp_3addr | ./cfg_cfg
		    elif [ $opt = 12 ]
		    then
			./cfg_dse_3addr | ./cfg_cfg
		    fi
		elif [ $value = "3addr" ]
		then
	            if [ $opt = 0 ]
		    then
			echo 1
		    elif [ $opt = 11 ]
		    then
			./cfg_scp_3addr
		    elif [ $opt = 12 ]
		    then
			./cfg_dce_3addr
		    fi
		elif [ $value = "rep" ]
		then
		    if [ $opt = 0 ]
		    then
			echo 1
		    elif [ $opt = 11 ]
		    then
			./cfg_scp_rep
		    elif [ $opt = 12 ]
		    then
			./cfg_dce_rep
		    fi
		fi
	elif [ $argument = "opt" ]
	then
		if [ $value = "scp" ]
		then
			opt=11
		elif [ $value = "dse" ]
		then
			opt=12
		fi		
	fi
done
