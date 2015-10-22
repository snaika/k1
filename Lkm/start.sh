#!/bin/bash

# Est-ce que l'utilisateur est root
if [ "$(id -u)" != "0" ];
then
    echo "Ce script doit être lancé par le root."
    echo sudo $0 $@ && sudo $0 $@ && exit
    exit -1
fi


insmod *.ko || exit -1
chmod 666 /dev/version
