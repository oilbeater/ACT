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
		elif [ $value = "ssa" ]
		then
		    if [ $opt = 0 ]
		    then
			./cfg_ssa
		    elif [ $opt = 13 ]
		    then
			./cfg_scp_ssa
		    elif [ $opt = 14 ]
		    then
			./cfg_dce_ssa
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
		    elif [ $opt = 13 ]
		    then
			./cfg_scp_ssa | ./ssa_3add
		    elif [ $opt = 14 ]
		    then
			./cfg_dce_ssa | ./ssa_3add

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
		    elif [ $opt = 13 ]
		    then
			./cfg_ssa_scp
		    elif [ $opt = 14 ]
		    then
			./cfg_ssa_dce
		    fi
		fi
	elif [ $argument = "opt" ]
	then
		if [ $value = "scp" ]
		then
			opt=11
		elif [ $value = "dce" ]
		then
			opt=12
		elif [ $value = "ssa,scp" ]
		then
			opt=13
		elif [ $value = "ssa,licm" ]
		then
			opt=14
		fi		
	fi
done
