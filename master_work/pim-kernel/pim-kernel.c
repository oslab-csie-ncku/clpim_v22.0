#include "linux_dcache.h"
#include "linux_list_bl.h"
#include "linux_nova.h"
#include "linux_pgtable.h"
#include "linux_rbtree.h"
#include "linux_seqlock.h"
#include "linux_stddef.h"
#include "linux_string.h"
#include "linux_stringhash.h"
#include "linux_tools.h"
#include "linux_types.h"
#include "pim-kernel.h"
#include "linux_direct_lookup.h"

uint64_t global_src;
uint64_t global_dst;
uint64_t global_total_size;

void init_reg(void)
{
    /*
    REG_FLUSH_ADDR = 0;
    REG_FLUSH_SIZE = 0;
    REG_FIRST_CMD = COMMAND_UNINIT;
    REG_0(0) = 0;
    REG_1(0) = 0;
    REG_2(0) = 0;
    REG_3(0) = 0;
    */
    memset_v((void *)SPM_START, 0, SPM_SIZE-1);
}

void kernel_nova_search_rbtree(volatile uint8_t cmd_index)
{
    // volatile const struct rb_root *tree = (struct rb_root *)REG_0(0);
    volatile struct rb_node *_node = (struct rb_node *)REG_0(cmd_index);
    const uint64_t hash_key = REG_1(cmd_index);
    volatile struct nova_range_node *curr = NULL;
    for (;
        _node && ({_node = (struct rb_node *)virt_to_phys(_node);
                    curr = container_of(_node, struct nova_range_node, node);
                    clflush((uint64_t)curr, sizeof(struct nova_range_node));
                    1;});
        _node = hash_key < curr->hash ? _node->rb_left : _node->rb_right) {
        if (hash_key == curr->hash) {
            REG_0(cmd_index) = 1;
            REG_1(cmd_index) = (uint64_t)curr;
            return;
        }
    }
    

    REG_0(cmd_index) = 0;
}

void kernel_dl_lookup_and_check(volatile uint8_t cmd_index)
{
    volatile struct dl_dir_lookup_entry *dl_entry = (struct dl_dir_lookup_entry *)REG_0(cmd_index);
    volatile uint64_t identity_hash = (uint64_t)REG_1(cmd_index);
    volatile uint64_t fs_uid_gid = (uint32_t)REG_2(cmd_index);
    volatile uint32_t fs_uid = (uint32_t)(fs_uid_gid >> 32);
    volatile uint32_t fs_gid = (uint32_t)fs_uid_gid;
    volatile int i;
	
    /* count the identity hash */
    /*
    for (i = 0; i < len; i++)
		identity_hash = identity_hash * 131 + (*str++);
    identity_hash += directory_hash;
    */

    /* search the DL hash table by the identity_hash*/
    for (; dl_entry; dl_entry = hlist_entry_safe(dl_entry->dl_hash.next, typeof(*(dl_entry)), dl_hash)) {
        dl_entry = (struct dl_dir_lookup_entry *)virt_to_phys(dl_entry);
        clflush((uint64_t)dl_entry, sizeof(struct dl_dir_lookup_entry));
		if (dl_entry->identity_hash != identity_hash)
			continue;
		break;
	}

    if (!dl_entry) {
        REG_0(cmd_index) = 0;
    }
    /* check permission */
    else {
        volatile struct dl_reachable_set *rset = (struct dl_reachable_set *)virt_to_phys(dl_entry->rset);
        for (i = 0; i < rset->num_entries; i++) {
            clflush((uint32_t)rset->entries[i].type, sizeof(enum dl_reachable_type));
            clflush((uint32_t)rset->entries[i].uid.val, sizeof(uint32_t));
            clflush((uint32_t)rset->entries[i].gid.val, sizeof(uint32_t));
            switch(rset->entries[i].type){
            case invalid:
                REG_0(cmd_index) = -1;
                break;
		    case other:
			    if((fs_uid == rset->entries[i].uid.val) || (fs_gid == rset->entries[i].gid.val))
				 REG_0(cmd_index) = -1;
		    case gid:
			    if(fs_gid != rset->entries[i].gid.val)
				    REG_0(cmd_index) = -1;
			    break;
            case not_uid:
                if(fs_uid == rset->entries[i].uid.val)
                    REG_0(cmd_index) = -1;
                break;
            case uid:
                if(fs_uid != rset->entries[i].uid.val)
                    REG_0(cmd_index) = -1;
                break;
            case not_gid:
                if(fs_gid != rset->entries[i].gid.val)
                    REG_0(cmd_index) = -1;
                break;
            case uid_or_gid:
                if(fs_uid != rset->entries[i].uid.val)
                    if(fs_gid != rset->entries[i].gid.val)
                        REG_0(cmd_index) = -1;
                break;
            default:
                continue;
            }
        }
        if (REG_0(cmd_index) != -1)
            REG_0(cmd_index) = dl_entry->ino;
    }
}
void kernel_nova_file_r(volatile uint8_t cmd_index)
{

     uint64_t src_addr = REG_0(cmd_index);
    uint64_t dst_addr = REG_1(cmd_index);
    uint64_t size = REG_2(cmd_index);
    uint64_t copysize;
   
      if(size>=4096-(dst_addr%4096)){
        copysize=4096-(dst_addr%4096);
    }else{
        copysize=size;
    }
    global_src=src_addr+copysize;
    global_dst=dst_addr+copysize;
     global_total_size=size-copysize;


    clflush(dst_addr,copysize);
    memcpy_v((void *)dst_addr, (void *)src_addr, copysize);
}

void kernel_nova_file_r_ext(volatile uint8_t cmd_index)
{

     uint64_t dst_addr = REG_0(cmd_index);
    uint64_t dst_addr1 = REG_1(cmd_index);
    uint64_t dst_addr2 = REG_2(cmd_index);
    uint64_t dst_addr3 = REG_3(cmd_index);
    uint64_t copysize;
    int buffer_counter=0;
   
    while(1){
        if(global_total_size>=4096-(global_dst%4096)){
            copysize=4096-(global_dst%4096);
        }else{
            copysize=global_total_size;
        }
        
            switch (buffer_counter)
            {
            case 0:
                global_dst=dst_addr;
                clflush(global_dst,copysize);
                    memcpy_v((void *)global_dst, (void *)global_src, copysize);
                    global_src+=copysize;
                    global_dst+=copysize;
                    global_total_size-=copysize;
                    if(global_dst%4096==0){ 
                        buffer_counter=1;
                }
                break;
            case 1:
                    global_dst=dst_addr1;
                    clflush(global_dst,copysize);
                    memcpy_v((void *)global_dst, (void *)global_src, copysize);
                    global_src+=copysize;
                    global_dst+=copysize;
                    global_total_size-=copysize;
                if(global_dst%4096==0){ 
                        buffer_counter=2;
                }
                break;
            case 2:
                    global_dst=dst_addr2;
                    clflush(global_dst,copysize);
                    memcpy_v((void *)global_dst, (void *)global_src, copysize);
                    global_src+=copysize;
                    global_dst+=copysize;
                    global_total_size-=copysize;
                if(global_dst%4096==0){ 
                        buffer_counter=3;
                }
                break;
            case 3:
                    global_dst=dst_addr3;
                    clflush(global_dst,copysize);
                    memcpy_v((void *)global_dst, (void *)global_src, copysize);
                    global_src+=copysize;
                    global_dst+=copysize;
                    global_total_size-=copysize;
                if(global_dst%4096==0){ 
                        buffer_counter=4;
                }
                break;
            default:
                break;
            }
            if(buffer_counter==4){
                break;
            }
            if(global_total_size==0){
                break;
            }
    }
    
}
void kernel_nova_file_w(volatile uint8_t cmd_index)
{
    uint64_t src_addr = REG_0(cmd_index);
    uint64_t dst_addr = REG_1(cmd_index);
    uint64_t size = REG_2(cmd_index);

    clflush(src_addr,size);
    memcpy_v((void *)dst_addr, (void *)src_addr, size);
}

void kernel_nova_file_w_ext(volatile uint8_t cmd_index)
{
    pgdval_t pgdval =  REG_0(cmd_index);
    uint64_t dst_phys_addr =  REG_1(cmd_index);
    uint64_t src_virt_addr =  REG_2(cmd_index);
    uint64_t size =  REG_3(cmd_index);
    // uint64_t round_size = REG_3;
    // uint64_t src_phys_addr =REG_2;
    // print("size %lu",size);
    
    while (size) {
        uint16_t round_max = PAGE_SIZE - (src_virt_addr & ~PAGE_MASK);
        uint16_t round_size;
        uint64_t src_phys_addr = user_virt_to_phys(pgdval, src_virt_addr,
                                                   clflush);

        if (size <= round_max)
            round_size = size;
        else
            round_size = round_max;
        size -= round_size;

        clflush(src_phys_addr, round_size);
        memcpy_v((void *)dst_phys_addr, (void *)src_phys_addr, round_size);

        dst_phys_addr += round_size;
        src_virt_addr += round_size;
    }
}
int pim_start()
{
    init_reg();
    volatile uint8_t cmd_index = 0;
    // Waiting for initialization info from OS
    while (REG_FIRST_CMD == COMMAND_UNINIT);
    // uint64_t cur_subjob_count=1;
    // uint64_t subjob_NUM=1;
    // Start working
    while (1) {
        const uint8_t cmd = REG_CMD(cmd_index);

        if (cmd >= COMMAND_NOVA_SEARCH_RBTREE && cmd <= COMMAND_NOVA_FILE_EXT) {
            REG_FIRST_CMD =cmd_index;
            switch (cmd) {
            case COMMAND_NOVA_SEARCH_RBTREE:
                kernel_nova_search_rbtree(cmd_index);
                REG_CMD(cmd_index) = COMMAND_DONE;
                if (cmd_index == MAX_JOB_NUM)
                    cmd_index = 0;
                else
                    cmd_index++;
                break;
            case COMMAND_NOVA_DL_LOOKUP:
                kernel_dl_lookup_and_check(cmd_index);
                REG_FIRST_CMD = COMMAND_DONE;
                REG_CMD(cmd_index) = COMMAND_DONE;
                if (cmd_index == MAX_JOB_NUM)
                    cmd_index = 0;
                else
                    cmd_index++;
                break;
            case COMMAND_NOVA_FILE_R:
                  kernel_nova_file_r(cmd_index);
                  REG_CMD(cmd_index) = COMMAND_DONE;
                  REG_FIRST_CMD =cmd_index;
                //   if (cmd_index ==( MAX_JOB_NUM-1))
                //     cmd_index = 0;
                // else
                //     cmd_index++;
                break;
            case COMMAND_NOVA_FILE_R_EXT:
                  kernel_nova_file_r_ext(cmd_index);
                  REG_CMD(cmd_index) = COMMAND_DONE;
                  REG_FIRST_CMD =cmd_index;
                //   if (cmd_index ==( MAX_JOB_NUM-1))
                //     cmd_index = 0;
                // else
                //     cmd_index++;
                break;
            case COMMAND_NOVA_FILE_W:
                kernel_nova_file_w(cmd_index);
                REG_CMD(cmd_index) = COMMAND_DONE;
                REG_FIRST_CMD =cmd_index;
                // if (cmd_index == (MAX_JOB_NUM-1))
                //     cmd_index = 0;
                // else
                //     cmd_index++;
                break;
            case COMMAND_NOVA_FILE_EXT:
                kernel_nova_file_w_ext(cmd_index);
                REG_CMD(cmd_index) = COMMAND_DONE;
                REG_FIRST_CMD =cmd_index;
                // if (cmd_index == (MAX_JOB_NUM-1))
                //     cmd_index = 0;
                // else
                //     cmd_index++;
                break;
            default:
                 REG_FIRST_CMD =cmd_index;
                ; // do nothing
            }
        }
    }

    return 0;
}
