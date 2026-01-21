#!/bin/bash
# === Copy pim_m5out from a given target directory into a fixed copy directory ===

# Check argument
if [ $# -ne 2 ]; then
    echo "Usage: $0 <target_dir> <ato/tim>"
    exit 1
fi

SRC_DIR="$1"
DEST_BASE="./copy_pim_m5out"
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")

# Validate source
if [ ! -d "$SRC_DIR" ]; then
    echo "Error: $SRC_DIR not found!"
    exit 1
fi

# Prepare destination folder
BASENAME=$(basename "$SRC_DIR")
DEST_DIR="$DEST_BASE/$2_${BASENAME}_$TIMESTAMP"
mkdir -p "$DEST_DIR"

# Copy
echo "Copying $SRC_DIR into  $DEST_DIR ..."
cp -r "$SRC_DIR/"* "$DEST_DIR/"

echo "Done! Backup stored at: $DEST_DIR"
ls -l $DEST_BASE/
