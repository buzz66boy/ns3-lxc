#!/bin/bash

for ((run=0; run<=$1; run++)); do
    br_name="$run_b"
    tap_name="$run_t"

    ifconfig $br_name down
    brctl delif $br_name $tap_name
    brctl delbr $br_name
    ifconfig $tap_name down
    ip tuntap del $tap_name mode tap
done

while : ; do
    veth=$(ifconfig | grep veth | cut -d " " -f1 | head -n1)
    [[ ! -z $veth ]] || break;
    ip link delete $veth
done