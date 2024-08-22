#!/bin/bash
# Program:
#	Build gem5, user can type
# History:
# 2021/11/26    johnnycck   First release
#!/bin/bash
        
source ./master_work/scripts/run/compile_gem5.sh

# if [ -z "$1" ]; then
# echo "Error: Missing $1, please fill in baseline or dlfs"
# exit 1
# fi
if [ -z "$1" ]; then
echo "Error: Missing $1, please fill in pim_system quantity"
exit 1
fi
if [ -z "$2" ]; then
echo "Error: Missing $2, please fill ceck"
exit 1
fi

ARCH=X86 #X86_MESI_Two_Level
GEM5_BIN_TYPE=opt # debug, opt, fast
BUILD_VARIABLES=''

export M5_PATH=./master_work/gem5_images/x86-system

GEM5_TARGET=./build/$ARCH/gem5.$GEM5_BIN_TYPE
FS_CONFIG=./configs/example/fs_pim.py


OUTDIR=pim_m5out
DEBUG_FLAGS=PseudoInst # CacheFlushRange, ScratchpadMemory


NUM_CPUS=1
CPU_TYPE=TimingSimpleCPU # AtomicSimpleCPU TimingSimpleCPU DerivO3CPU
RESTORE_CPU_TYPE=AtomicSimpleCPU
CPU_CLOCK=2GHz


L1I_SIZE=32kB
L1D_SIZE=32kB
L2_SIZE=1MB


MEM_TYPE=DDR4_2400_16x4
MEM_CHANNELS=1
MEM_SIZE=16GB
NVM_TYPE=PCM_LPDDR2_400_8x8

# Must match the memmap in the kernel cmdline (?G!?G). Be careful with x86 3G hole
NVM_START=0x240000000 # 9G
NVM_SIZE=8GB


PIM_CPU_CLOCK=1.5GHz
PIM_L1I_CACHE_SIZE=4kB
PIM_L1D_CACHE_SIZE=4kB
PIM_BANDWIDTH_RATIO=8
PIM_SPM_START=0x450000000 # Must be after the SE memory range
PIM_SPM_SIZE=4kB # The minimum memory size is page size, but it won't actually be used so much
PIM_SPM_REG_FLUSH_ADDR=0x450000000
PIM_SPM_REG_FLUSH_SIZE=0x450000008
PIM_SE_MEM_START=0x440000000 # Must be after the host physical memory range
PIM_SE_MEM_SIZE=512kB # The min size for gem5 setting, but we use less than 16kB
#PIM_KERNEL=/home/johnnylab/work/gem5_images/pim-kernel
PIM_KERNEL=/home/a1933b/gem5-clpim_v22.0.0.0/master_work/pim-kernel/pim-kernel
PIM_SE_INPUT=''
PIM_SE_OUTPUT=pim-stdout
PIM_SE_ERROUT=pim-errout
PIM_STACK_NUM=$1
CHECK_POINT_NUM=$2

# /lib/modules/4.18.0+/kernel/fs/nova/nova.ko
# ./master_work/workloads/metadata/
# ./master_work/workloads/data/
# ./master_work/workloads/real/
SCRIPT=./master_work/workloads/real/fileserver.f

#sudo bash  ./mount.sh /home/a1933b/gem5-pim/master_work/gem5_images/x86-system/disks/x86-example.img s_tmp
KERNEL=/home/a1933b/gem5-clpim_v22.0.0.0/master_work/gem5_images/x86-system/binaries/x86_64-vmlinux-4.18.0-nova-pohao-baseline-$1
# KERNEL=x86_64-vmlinux-4.18.0-nova-pohao-$1-$2
CMDLINE="earlyprintk=ttyS0 console=ttyS0 lpj=7999923 root=/dev/hda1 ddlhash_entries=131072 nokaslr norandmaps memmap=8G!9G"
CMDLINE="earlyprintk=ttyS0 console=ttyS0 lpj=7999923 root=/dev/hda1 nokaslr norandmaps memmap=8G!9G"

# DISK_IMAGE=x86-example.img
#DISK_IMAGE=/home/johnnylab/work/gem5_images/x86-filebench_syscall.img
DISK_IMAGE=/home/a1933b/gem5-clpim_v22.0.0.0/master_work/gem5_images/x86-system/disks/x86-example.img
#3456 cpu read sucess and same vaddr
#3457 dpu read no cache

 "$GEM5_TARGET" \
    --outdir="$OUTDIR" \
    --debug-flags="$DEBUG_FLAGS" \
    "$FS_CONFIG" \
    --num-cpus="$NUM_CPUS" \
    --cpu-type="$CPU_TYPE" \
    --cpu-clock="$CPU_CLOCK" \
    --caches \
    --l2cache \
    --l1i_size="$L1I_SIZE" \
    --l1d_size="$L1D_SIZE" \
    --l2_size="$L2_SIZE" \
    --mem-type="$MEM_TYPE" \
    --mem-channels="$MEM_CHANNELS" \
    --mem-size="$MEM_SIZE" \
    --dram-nvm \
    --dram-nvm-type="$NVM_TYPE" \
    --dram-nvm-start="$NVM_START" \
    --dram-nvm-size="$NVM_SIZE" \
    --pim-stack-num="$PIM_STACK_NUM" \
    --pim-se \
    --hybrid-stack \
    --pim-cpu-clock="$PIM_CPU_CLOCK" \
    `#--pim-l1i-cache-size="$PIM_L1I_CACHE_SIZE"` \
    `#--pim-l1d-cache-size="$PIM_L1D_CACHE_SIZE"` \
    --pim-bandwidth-ratio="$PIM_BANDWIDTH_RATIO" \
    --pim-spm-start="$PIM_SPM_START" \
    --pim-spm-size="$PIM_SPM_SIZE" \
    --pim-spm-reg-flush-addr="$PIM_SPM_REG_FLUSH_ADDR" \
    --pim-spm-reg-flush-size="$PIM_SPM_REG_FLUSH_SIZE" \
    --pim-se-mem-start="$PIM_SE_MEM_START" \
    --pim-se-mem-size="$PIM_SE_MEM_SIZE" \
    --pim-kernel="$PIM_KERNEL" \
    --pim-se-input="$PIM_SE_INPUT" \
    --pim-se-output="$PIM_SE_OUTPUT" \
    --pim-se-errout="$PIM_SE_ERROUT" \
   --checkpoint-restore="$CHECK_POINT_NUM"  \
    --restore-with-cpu="$RESTORE_CPU_TYPE" \
    --script="$SCRIPT" \
    --kernel="$KERNEL" \
    --command-line="$CMDLINE" \
    --disk-image="$DISK_IMAGE"

read -p "Enter 0 to exit or 1 to continue: " choice

# 判断用户输入是否为1，如果不是则退出脚本
if [ "$choice" != "1" ]; then
    echo "Exiting..."
    exit 1
fi
source_dir="./pim_m5out/stats.txt"
source_dir_pimkernel="./master_work/pim-kernel"
source_dir_nova="../linux-nova-multipim/fs/nova"
source_dir_driver="../linux-nova-multipim/drivers/pim"

target_dir="./timing_test"

nova_filenames="file.c multistack_read_write.h"
drivers_filenames="pim.c pim.h"
pimkernel_filenames="pim-kernel.c pim-kernel.h"
nova_filename=($nova_filenames)
drivers_filename=($drivers_filenames)
pimkernel_filename=($pimkernel_filenames)
# 获取目标目录中已经存在的目录列表
existing_dirs=$(ls -d "$target_dir"/*/ 2>/dev/null)

# 如果没有已存在的目录，则从 1 开始编号，否则找到最大的编号并加 1
if [ -z "$existing_dirs" ]; then
    count=1
else
    # 提取目录名称中的数字部分，找到最大值
    max_count=0
    for dir in $existing_dirs; do
        dir_name=$(basename "$dir")
        num=${dir_name%%-*}  # 提取目录名称中的数字部分
        if [ "$num" -gt "$max_count" ]; then
            max_count=$num
        fi
    done
    ((count = max_count + 1))
fi

# 创建新目录名
timestamp=$(date "+%m%d-%H%M")
new_dir_name="$count-$timestamp"
new_dir="$target_dir/$new_dir_name"

new_dir_nova="$target_dir/$new_dir_name/nova"
new_dir_driver="$target_dir/$new_dir_name/driver"
new_dir_pim_kernel="$target_dir/$new_dir_name/pim-kerne"
# 创建新目录
mkdir -p "$new_dir"
mkdir -p "$new_dir_nova"
mkdir -p "$new_dir_driver"
mkdir -p "$new_dir_pim_kernel"

# 复制源目录到新目录
for file in "${nova_filename[@]}"; do
    cp "$source_dir_nova/$file" "$new_dir_nova"
    echo "Copying $file to $nenew_dir_novaw_dir"
done
for file in "${pimkernel_filename[@]}"; do
    cp "$source_dir_pimkernel/$file" "$new_dir_pim_kernel"
    echo "Copying $file to $new_dir_pim_kernel"
done
for file in "${drivers_filename[@]}"; do
    cp "$source_dir_driver/$file" "$new_dir_driver"
    echo "Copying $file to $new_dir_driver"
done

cp -r "$source_dir" "$new_dir"
echo "Copied $source_dir to $new_dir"

total_size=$(find "$target_dir" -type f -exec du -b {} + | awk '{ total += $1 } END { print total }')

# 将总大小转换为 MB
total_size_mb=$(echo "scale=2; $total_size / 1024 / 1024" | bc)

echo "Total size of files in all directories under $target_dir: $total_size_mb MB"
echo " print stats.txt"
python3 out.py