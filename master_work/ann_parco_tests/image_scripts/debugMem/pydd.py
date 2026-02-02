#!/usr/bin/env python3
import os
import sys
import argparse
import subprocess

# --- Configuration ---
# Must match your gem5 memory map / kernel cmdline
NVM_START = 0x440000000 
DEFAULT_DEVICE = "/dev/pmem0"

def read_pmem_raw(device_path, offset, size=64):
    """
    Reads data from pmem device using O_DIRECT.
    Handles memory alignment automatically for the user.
    """
    try:
        # Open with O_DIRECT to bypass OS Page Cache
        fd = os.open(device_path, os.O_RDONLY | os.O_DIRECT)
        
        # O_DIRECT requires 512B or 4KB alignment.
        # We align down to the nearest 4KB (page size) boundary.
        aligned_offset = (offset // 4096) * 4096
        padding = offset - aligned_offset
        
        # Ensure we read enough blocks to cover the requested range
        read_len = ((padding + size + 4095) // 4096) * 4096
        
        os.lseek(fd, aligned_offset, os.SEEK_SET)
        raw_buffer = os.read(fd, read_len)
        os.close(fd)
        
        return raw_buffer[padding : padding + size]
    
    except PermissionError:
        print(f"[!] Error: Permissions denied. Please run with 'sudo'.")
        sys.exit(1)
    except OSError as e:
        print(f"[!] OS Error: {e}. Check if {device_path} exists.")
        sys.exit(1)

def run_m5_writefile(filename):
    """Executes the m5 writefile command to send data to the Host."""
    try:
        # Change file permissions so the m5 utility (often running as guest user) can read it
        os.chmod(filename, 0o666)
        subprocess.run(["m5", "writefile", filename], check=True)
        print(f"[+] Successfully sent '{filename}' to Host m5out/ directory.")
    except FileNotFoundError:
        print("[!] Warning: 'm5' command not found in guest path. File saved locally only.")
    except subprocess.CalledProcessError:
        print("[!] Error: 'm5 writefile' failed.")

def print_hexdump(data, offset, phys_addr):
    """Standard hex + ascii visualization."""
    print(f"\n{'='*70}")
    print(f"Physical Addr: {hex(phys_addr)} | Device Offset: {hex(offset)}")
    print(f"{'='*70}")
    for i in range(0, len(data), 16):
        chunk = data[i:i+16]
        hex_part = " ".join(f"{b:02x}" for b in chunk)
        ascii_part = "".join(chr(b) if 32 <= b <= 126 else "." for b in chunk)
        print(f"{i:08x}  {hex_part:<48}  |{ascii_part}|")
    print(f"{'='*70}\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="PIM/NVM Debugger: Extract memory data")
    
    # Address selection
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-p", "--physical", help="Target Physical Address (Hex)")
    group.add_argument("-o", "--offset", help="Target Device Offset (Hex)")
    
    # Optional parameters
    parser.add_argument("-s", "--size", type=int, default=64, help="Bytes to read (Default: 64)")
    parser.add_argument("-f", "--file", help="Output filename (if provided, saves data and triggers m5 writefile)")
    parser.add_argument("-d", "--device", default=DEFAULT_DEVICE, help=f"Input device (Default: {DEFAULT_DEVICE})")

    args = parser.parse_args()

    try:
        # Calculate offsets
        if args.physical:
            target_phys = int(args.physical, 16)
            target_offset = target_phys - NVM_START
        else:
            target_offset = int(args.offset, 16)
            target_phys = target_offset + NVM_START

        if target_offset < 0:
            print(f"[!] Error: Calculated offset {hex(target_offset)} is out of range.")
            sys.exit(1)

        # Read data from hardware
        data_block = read_pmem_raw(args.device, target_offset, args.size)

        # If a filename is provided, save it and transfer via m5
        if args.file:
            with open(args.file, "wb") as f:
                f.write(data_block)
            print(f"[+] Data saved to {args.file}")
            run_m5_writefile(args.file)
        else:
            # Default behavior: just show hexdump
            print_hexdump(data_block, target_offset, target_phys)

    except ValueError:
        print("[!] Error: Invalid hex format. Ensure addresses start with '0x'.")
        sys.exit(1)

