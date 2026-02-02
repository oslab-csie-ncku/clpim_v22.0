#! /bin/bash

m5 checkpoint
echo "M5 checkpoint 1"
mount -t NOVA -o init /dev/pmem0 /nova
echo "Mount NOVA at /dev/pmem0 which is /nova"
cp *.sh /nova/
#mkdir -p /nova/d4/d3/d2
#mkdir -p /nova/d9/d8/d7/d6/d5/d4/d3/d2
echo "start copying to d1"
cp -r d1 /nova/
cp /root/ann_tests/* /nova/
echo "copy files done"
#cd /nova
#echo "start copying to d4"
#cp -r d1 /nova/d4/d3/d2/
cd /nova
#echo "start copying to d9"
#cp -r d1 d9/d8/d7/d6/d5/d4/d3/d2/
m5 checkpoint
echo "M5 checkpoint 2"
ls -a
