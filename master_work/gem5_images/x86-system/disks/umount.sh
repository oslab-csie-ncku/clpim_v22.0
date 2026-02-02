#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Usage: $0 <mount_dir>"
    exit 1
fi

sudo umount $1