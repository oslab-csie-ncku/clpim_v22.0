#!/bin/bash

# --- Configuration ---
NVM_START=0x440000000

usage() {
    echo "Description: Extract 4KB from NVM and send to Host"
    echo "Usage: $0 [option] <address> <output_file> [input_device]"
    echo "Options:"
    echo "  -p    Use absolute Physical Address (e.g., 0x4b9eb8000)"
    echo "  -o    Use direct Offset from pmem0 (e.g., 0x79eb8000)"
    echo "Example:"
    echo "  $0 -p 0x4b9eb8000 result.bin /dev/pmem0"
    exit 1
}

if [ "$#" -lt 3 ] || [ "$#" -gt 4 ]; then
    usage
fi

OPTION=$1
ADDR=$2
OUTFILE=$3
INPUTF=${4:-"/dev/pmem0"}

# Logic: Calculate Offset
if [ "$OPTION" == "-p" ]; then
    OFFSET=$(( ADDR - NVM_START ))
    echo "Target: Physical $ADDR -> Calculated Offset: $(printf "0x%llx" $OFFSET)"
elif [ "$OPTION" == "-o" ]; then
    OFFSET=$(( ADDR ))
    echo "Target: Direct Offset $(printf "0x%llx" $OFFSET)"
else
    usage
fi

# Safety Check
if [[ $OFFSET -lt 0 ]]; then
    echo "Error: Calculated offset is negative. Check your physical address."
    exit 1
fi

echo "Extracting 4KB from $INPUTF to $OUTFILE..."

# Execute dd
sudo dd if="$INPUTF" bs=4096 count=1 skip=$OFFSET iflag=skip_bytes,direct of="$OUTFILE"

if [ $? -eq 0 ]; then
    sudo chmod 666 "$OUTFILE"
    # Transfer to gem5 Host
    m5 writefile "$OUTFILE"
    echo "Success! File '$OUTFILE' transferred to Host m5out/ directory."
else
    echo "Error: dd command failed."
    echo "Check if $INPUTF exists and if the address is 512-byte aligned."
fi

