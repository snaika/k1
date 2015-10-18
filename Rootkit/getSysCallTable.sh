#!/bin/bash

KERNEL_VERSION=`uname -r`
BOOT_SYSTEM_KERNEL=`find /boot/ -name "System.map-${KERNEL_VERSION}"`

if [[ -e "${BOOT_SYSTEM_KERNEL}" ]]; then
    SYSCALL_TABLE=`grep sys_call_table ${BOOT_SYSTEM_KERNEL} | grep " R " | grep --invert-match ia32_sys_call_table | sed 's/R sys_call_table[[:space:]]*//' | sed 's/^/0x/' | sed 's/[[:space:]]*\$//'`
    echo "sys_call_table :" ${SYSCALL_TABLE}
    sed "s/JE_SUIS_UN_SYSCALL_A_CHANGER/${SYSCALL_TABLE}/" NewOpen.tochange.c > NewOpen.c || exit -1
else
    echo "Invalid / incorrect System.map-... : ${BOOT_SYSTEM_KERNEL}"
fi
