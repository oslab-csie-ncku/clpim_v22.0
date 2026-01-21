// #ifndef __REGS_H__
// #define __REGS_H__

// #include "linux_types.h"

// /* pim_system: 0 */
// #ifdef DPIM0
// #define SPM_START       (0x450000000)
// #define REG_BASE        (0x45000000d)
// #endif
// /* pim_system: 1 */
// #ifdef DPIM1
// #define SPM_START       (0x450001000)
// #define REG_BASE        (0x45000100d)
// #endif
// /* pim_system: 2 */
// #ifdef DPIM2
// #define SPM_START       (0x450002000)
// #define REG_BASE        (0x45000200d)
// #endif
// /* pim_system: 3 */
// #ifdef DPIM3
// #define SPM_START       (0x450003000)
// #define REG_BASE        (0x45000300d)
// #endif
// #define SPM_SIZE        ((1) << (12UL)) /* 4096 bytes*/

// /* Register offset for each job relative to REG_0 */
// #define REG_FLUSH_SIZE_OFFSET   0x08
// #define REG_FIRST_CMD_OFFSET   0x0c

// #define REG_CMD_OFFSET	        0x00
// #define REG_0_OFFSET		    0x01
// #define REG_1_OFFSET		    0x09
// #define REG_2_OFFSET		    0x11
// #define REG_3_OFFSET		    0x19
// #define PIM_JOB_SIZE            33
// #define MAX_JOB_NUM		        ((SPM_SIZE-12) / PIM_JOB_SIZE)

// /**
//  * Register map(fixed)
//  */
// #define REG_FLUSH_ADDR *((volatile uint64_t *)(SPM_START))
// #define REG_FLUSH_SIZE *((volatile uint32_t *)(SPM_START + REG_FLUSH_SIZE_OFFSET))
// #define REG_FIRST_CMD  *((volatile uint8_t  *)(SPM_START + REG_FIRST_CMD_OFFSET))

// #define REG_CMD(N)     *((volatile uint8_t  *)(REG_BASE + ((N) * PIM_JOB_SIZE) + REG_CMD_OFFSET)) /* 1 bytes */
// #define REG_0(N)       *((volatile uint64_t *)(REG_BASE + ((N) * PIM_JOB_SIZE) + REG_0_OFFSET)) /* 8 bytes */
// #define REG_1(N)       *((volatile uint64_t *)(REG_BASE + ((N) * PIM_JOB_SIZE) + REG_1_OFFSET)) /* 8 bytes */
// #define REG_2(N)       *((volatile uint64_t *)(REG_BASE + ((N) * PIM_JOB_SIZE) + REG_2_OFFSET)) /* 8 bytes */
// #define REG_3(N)       *((volatile uint64_t *)(REG_BASE + ((N) * PIM_JOB_SIZE) + REG_3_OFFSET)) /* 8 bytes */

// /**
//  * The size of each register
//  */
// #define REG_CMD_SIZE	1
// #define REG_0_SIZE		8
// #define REG_1_SIZE		8
// #define REG_2_SIZE		8
// #define REG_3_SIZE		8

// #endif /* __REGS_H__ */

#ifndef __REGS_H__
#define __REGS_H__

#include "linux_types.h"

/* pim_system: 0 */
#ifdef DPIM0
#define SPM_START       (0x850000000)
#define REG_BASE        (0x85000000d)
#endif
/* pim_system: 1 */
#ifdef DPIM1
#define SPM_START       (0x850001000)
#define REG_BASE        (0x85000100d)
#endif
/* pim_system: 2 */
#ifdef DPIM2
#define SPM_START       (0x850002000)
#define REG_BASE        (0x85000200d)
#endif
/* pim_system: 3 */
#ifdef DPIM3
#define SPM_START       (0x850003000)
#define REG_BASE        (0x85000300d)
#endif

#ifdef DPIM4
#define SPM_START       (0x850004000)
#define REG_BASE        (0x85000400d)
#endif

#ifdef DPIM5
#define SPM_START       (0x850005000)
#define REG_BASE        (0x85000500d)
#endif

#ifdef DPIM6
#define SPM_START       (0x850006000)
#define REG_BASE        (0x85000600d)
#endif

#ifdef DPIM7
#define SPM_START       (0x850007000)
#define REG_BASE        (0x85000700d)
#endif

#define SPM_SIZE        ((1) << (12UL)) /* 4096 bytes*/

/* Register offset for each job relative to REG_0 */
#define REG_FLUSH_SIZE_OFFSET   0x08
#define REG_FIRST_CMD_OFFSET   0x0c

#define REG_CMD_OFFSET	        0x00
#define REG_0_OFFSET		    0x01
#define REG_1_OFFSET		    0x09
#define REG_2_OFFSET		    0x11
#define REG_3_OFFSET		    0x19
#define PIM_JOB_SIZE            33
#define MAX_JOB_NUM		        ((SPM_SIZE-13) / PIM_JOB_SIZE)
// MAX_JOB_NUM is 123
/**
 * Register map(fixed)
 */
#define REG_FLUSH_ADDR *((volatile uint64_t *)(SPM_START))
#define REG_FLUSH_SIZE *((volatile uint32_t *)(SPM_START + REG_FLUSH_SIZE_OFFSET))
#define REG_FIRST_CMD  *((volatile uint8_t  *)(SPM_START + REG_FIRST_CMD_OFFSET))

#define REG_CMD(N)     *((volatile uint8_t  *)(REG_BASE + ((N) * PIM_JOB_SIZE) + REG_CMD_OFFSET)) /* 1 bytes */
#define REG_0(N)       *((volatile uint64_t *)(REG_BASE + ((N) * PIM_JOB_SIZE) + REG_0_OFFSET)) /* 8 bytes */
#define REG_1(N)       *((volatile uint64_t *)(REG_BASE + ((N) * PIM_JOB_SIZE) + REG_1_OFFSET)) /* 8 bytes */
#define REG_2(N)       *((volatile uint64_t *)(REG_BASE + ((N) * PIM_JOB_SIZE) + REG_2_OFFSET)) /* 8 bytes */
#define REG_3(N)       *((volatile uint64_t *)(REG_BASE + ((N) * PIM_JOB_SIZE) + REG_3_OFFSET)) /* 8 bytes */

/**
 * The size of each register
 */
#define REG_CMD_SIZE	1
#define REG_0_SIZE		8
#define REG_1_SIZE		8
#define REG_2_SIZE		8
#define REG_3_SIZE		8

#endif /* __REGS_H__ */
