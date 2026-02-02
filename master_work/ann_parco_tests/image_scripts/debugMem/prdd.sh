#!/bin/bash

# --- Configuration ---
NVM_START=0x440000000

usage() {
    echo "Description: Quick hexdump view of NVM data"
    echo "Usage: $0 [option] <address> [input_device]"
    echo "Options:"
    echo "  -p    Use absolute Physical Address (e.g., 0x4b9eb8000)"
    echo "  -o    Use direct Offset from device start (e.g., 0x79eb8000)"
    echo "Example:"
    echo "  $0 -p 0x4b9eb8000 /dev/pmem0"
    exit 1
}

# Check argument count: expects 2 or 3 arguments
if [ "$#" -lt 2 ] || [ "$#" -gt 3 ]; then
    usage
fi

OPTION=$1
ADDR=$2
# Default to /dev/pmem0 if the 3rd argument is missing
INPUTF=${3:-"/dev/pmem0"}

# Logic: Address Translation
if [ "$OPTION" == "-p" ]; then
    PHY_ADDRESS=$(( ADDR ))
    OFFSET=$(( PHY_ADDRESS - NVM_START ))
elif [ "$OPTION" == "-o" ]; then
    OFFSET=$(( ADDR ))
    PHY_ADDRESS=$(( OFFSET + NVM_START ))
else
    usage
fi

# Check for out-of-bounds
if [[ $OFFSET -lt 0 ]]; then
    echo "Error: Offset is negative. Check your physical address."
    exit 1
fi

# Display access metadata
echo "Accessing Physical: $(printf "0x%llx" $PHY_ADDRESS) | Offset: $(printf "0x%llx" $OFFSET) | Device: $INPUTF"

# Execute dd and pipe to hexdump
# We limit to 'head -c 64' to keep the output concise
sudo dd if="$INPUTF" bs=4096 count=1 skip=$OFFSET iflag=skip_bytes,direct | head -c 64 | hexdump -C
