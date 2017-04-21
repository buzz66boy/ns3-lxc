#!/bin/bash

br_name="$1_br"
tap_name="$1_tap"

ifconfig $br_name down
brctl delif $br_name $tap_name
brctl delbr $br_name
ifconfig $tap_name down
ip tuntap del $tap_name mode tap

while : ; do
    veth=$(ifconfig | grep veth | cut -d " " -f1 | head -n1)
    [[ ! -z $veth ]] || break;
    ip link delete $veth
done