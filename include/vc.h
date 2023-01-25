#ifndef _VC_H
#define _VC_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef VC_VERSION
#error no vc version specified
#endif

#define NACJ 0
#define NACE 1
#define NARJ 2
#define NARE 3
#define NAEJ 4
#define NAEE 5

#define IS_MM   (VC_VERSION == NARJ || VC_VERSION == NARE)
#define IS_OOT  (VC_VERSION == NACJ || VC_VERSION == NACE)
#define IS_PM   (VC_VERSION == NAEJ || VC_VERSION == NAEE)

#define ENTRY   __attribute__((section(".init")))

typedef bool (*lb_t)(void *dev_obj, uint32_t addr, uint8_t *dest);
typedef bool (*lh_t)(void *dev_obj, uint32_t addr, uint16_t *dest);
typedef bool (*lw_t)(void *dev_obj, uint32_t addr, uint32_t *dest);
typedef bool (*ld_t)(void *dev_obj, uint32_t addr, uint64_t *dest);
typedef bool (*sb_t)(void *dev_obj, uint32_t addr, uint8_t *src);
typedef bool (*sh_t)(void *dev_obj, uint32_t addr, uint16_t *src);
typedef bool (*sw_t)(void *dev_obj, uint32_t addr, uint32_t *src);
typedef bool (*sd_t)(void *dev_obj, uint32_t addr, uint64_t *src);
typedef bool (*unk_2c_t)(void *dev_obj, uint32_t addr, void*);

enum ppc_exception {
    EX_RESET,
    EX_MACH_CHECK,
    EX_DSI,
    EX_ISI,
    EX_EXT_INTR,
    EX_ALIGN,
    EX_PROG,
    EX_FP_UNAVAIL,
    EX_DECR,
    EX_SYSCALL,
    EX_TRACE,
    EX_MAX
};

typedef struct{
    char        unk_0x00_[0x04];    /* 0x00 */
    void       *dev_obj;            /* 0x04 */
    uint32_t    kseg0_offset;       /* 0x08 */
    lb_t        lb;                 /* 0x0C */
    lh_t        lh;                 /* 0x10 */
    lw_t        lw;                 /* 0x14 */
    ld_t        ld;                 /* 0x18 */
    sb_t        sb;                 /* 0x1C */
    sh_t        sh;                 /* 0x20 */
    sw_t        sw;                 /* 0x24 */
    sd_t        sd;                 /* 0x28 */
#if (IS_OOT || IS_PM)
    unk_2c_t    unk_0x2C;           /* 0x2C */
    uint32_t    n64_vram_start;     /* 0x30 */
    uint32_t    n64_vram_end;       /* 0x34 */
    uint32_t    n64_pram_start;     /* 0x38 */
    uint32_t    n64_pram_end;       /* 0x3C */
#elif IS_MM
    uint32_t    phys_addr;          /* 0x2C */
    uint32_t    size;               /* 0x30 */
#endif
} cpu_dev_t;

typedef struct gClassCPU_s gClassCPU_t;
typedef struct func_tree_node_s func_tree_node_t;

struct func_tree_node_s{
    char                unk_0x00[4];    /* 0x00 */
    void               *code;           /* 0x04 */
    int                 branch_cnt;     /* 0x08 */
    void               *branches;       /* 0x0C */
    uint32_t            start_addr;     /* 0x10 */
    uint32_t            end_addr;       /* 0x14 */
    void               *addr_ref;       /* 0x18 */
    uint32_t            ref_cnt;        /* 0x1C */
    uint32_t            status;         /* 0x20 */
    uint32_t            checksum;       /* 0x24 */
    uint32_t            timer;          /* 0x28 */
    size_t              size;           /* 0x2C */
    int                 alloc_type;     /* 0x30 */
    int                 block_pos;      /* 0x34 */
    int                 node_pos;       /* 0x38 */
    func_tree_node_t   *parent;         /* 0x3C */
    func_tree_node_t   *left;           /* 0x40 */
    func_tree_node_t   *right;          /* 0x48 */
};                                      /* 0x4C */

typedef struct {
    uint16_t            node_cnt;           /* 0x00 */
    char                unk_0x02[2];        /* 0x02 */
    uint32_t            total_size;         /* 0x04 */
    int                 code_addr;          /* 0x08 */
    uint32_t            addr_start;         /* 0x0C */
    uint32_t            code_end;           /* 0x10 */
    char                unk_0x14[0x54];     /* 0x14 */
    func_tree_node_t   *code_root;          /* 0x68 */
    func_tree_node_t   *other_root;         /* 0x6C */
    char                unk_0x70[0x14];     /* 0x70 */
} func_tree_ctx_t;                          /* 0x80 */

#if IS_OOT

typedef struct{
    char        unk_0x00[0x10];
    gClassCPU_t  *cpu;
} gClassSystem_t;

struct gClassCPU_s{
    char        unk_0x00_[0xB60];
    cpu_dev_t  *cpu_devs[0x100];
#if VC_VERSION == NACE
    uint8_t     dev_idx[0x100000];
#else
    uint8_t     dev_idx[0x10000];
#endif
    void       *recompiler_1;
    void       *recompiler_2;
    char        unk_0x10F68[0x1370];
} ;

#elif IS_MM

typedef struct{
    char        unk_0x00[0x28];
    gClassCPU_t  *cpu;
} gClassSystem_t;

struct gClassCPU_s {
    uint32_t            status;                     /* 0x00000 */
    char                unk_0x04[0x14];             /* 0x00004 */
    gClassSystem_t     *sys;                        /* 0x00018 */
    char                unk_0x1C[0x4];              /* 0x0001C */
    uint32_t            lo[2];                      /* 0x00020 */
    uint32_t            hi[2];                      /* 0x00028 */
    uint32_t            cache_cnt;                  /* 0x00030 */
    uint8_t             phys_ram_dev_idx;           /* 0x00034 */
    uint32_t            pc;                         /* 0x00038 */
    char                unk_0x3C[0x10];             /* 0x0003C */
    func_tree_node_t   *working_node;               /* 0x0004C */
    char                unk_0x50[4];                /* 0x00050 */
    int                 timer;                      /* 0x00054 */
    union {
        uint64_t        gpr64[32];
        uint32_t        gpr[64];
    };                                              /* 0x00058 */
    union {
        double          fpr64[32];
        float           fpr[64];
    };                                              /* 0x00158 */
    char                unk_0x258[0x780];           /* 0x00258 */
    uint32_t            FSCR[32];                   /* 0x009D8 */
    uint32_t            cp0[64];                    /* 0x00A58 */
    void               *exec_opcode_func;           /* 0x00B58 */
    void               *exec_jump_func;             /* 0x00B5C */
    void               *exec_call_func;             /* 0x00B60 */
    void               *exec_idle_func;             /* 0x00B64 */
    void               *exec_load_store_func;       /* 0x00B68 */
    void               *exec_fp_load_store_func;    /* 0x00B6C */
    uint32_t            time_hi;                    /* 0x00B70 */
    uint32_t            time_lo;                    /* 0x00B74 */
    char                unk_0xB78[8];               /* 0x00B78 */
    cpu_dev_t          *cpu_devs[256];              /* 0x00B80 */
    uint8_t             dev_idx[0x10000];           /* 0x00F80 */
    void               *sm_blk_code;                /* 0x10F80 */
    void               *lg_blk_code;                /* 0x10F84 */
    uint32_t            sm_blk_alloc[256];          /* 0x10F88 */
    uint32_t            lg_blk_alloc[13];           /* 0x11388 */
    func_tree_ctx_t    *tree_ctx;                   /* 0x113BC */
    char                unk_0x113C0[0xDC0];         /* 0x113C0 */
    uint32_t            known_regs;                 /* 0x12180 */
    char                unk_0x12184[8];             /* 0x12184 */
    uint32_t            jr_is_ra;                   /* 0x1218C */
    char                unk_0x12190[0x18];          /* 0x12190 */
    uint32_t            prev_loadstore_base;        /* 0x121A8 */
    char                unk_0x121AC[0x14];          /* 0x121AC */
};                                                  /* 0x121C0 */

#elif IS_PM

typedef struct {
    char        unk_0x00[0x18];
    gClassCPU_t  *cpu;
} gClassSystem_t;

struct gClassCPU_s {
    char        unk_0x00_[0xB60];
    cpu_dev_t  *cpu_devs[0x100];
    uint8_t     dev_idx[0x100000];
    void       *recompiler_1;
    void       *recompiler_2;
    char        unk_0x10F68[0x1370];
};
#endif

typedef struct {
    const char *name;
    size_t size;
    int unk_08;
    int (*event_handler)(void *heap, int event, void *arg);
} class_type_t;

typedef struct thread_context_s     thread_context_t;

struct thread_context_s {
    /* 0x0000 */ uint32_t gpr[32];
    /* 0x0080 */ uint32_t cr;
    /* 0x0084 */ uint32_t lr;
    /* 0x0088 */ uint32_t ctr;
    /* 0x008C */ uint32_t xer;
    union {
        /* 0x0090 */ double fpr[32];
        /* 0x0090 */ uint64_t fpr_64[32];
    };
    /* 0x0190 */ uint64_t fscr;
    /* 0x0198 */ uint32_t srr0;
    /* 0x019C */ uint32_t srr1;
    /* 0x01A0 */ char unk_0x1A0[8];
    /* 0x01A8 */ uint32_t gqr[7];
    /* 0x01C8 */ uint64_t ps[32];
    /* 0x02C8 */ char unk_0x2C8[0x14];
    /* 0x02DC */ thread_context_t **queue;
    /* 0x02E0 */ thread_context_t *next;
    /* 0x02E4 */ thread_context_t *prev;
    /* 0x02E8 */ char unk_0x2E8[0x1C];
    /* 0x0304 */ void *stack_start;
    /* 0x0308 */ void *stack_end;
    /* 0x030C */ char unk_0x30C[12];
}; // size = 0x318

#if VC_VERSION == NACJ
#define init_hook_addr          0x800078E8
#define ios_openasync_addr      0x800b884c
#define ios_open_addr           0x800b8964
#define ios_closeasync_addr     0x800b8a84
#define ios_close_addr          0x800b8b44
#define ios_readasync_addr      0x800b8bec
#define ios_read_addr           0x800b8cec
#define ios_writeasync_addr     0x800b8df4
#define ios_write_addr          0x800b8ef4
#define ios_seekasync_addr      0x800b8ffc
#define ios_seek_addr           0x800b90dc
#define ios_ioctlasync_addr     0x800b91c4
#define ios_ioctl_addr          0x800b92fc
#define ios_ioctlvasync_addr    0x800b9568
#define ios_ioctlv_addr         0x800b964c
#define ios_create_heap_addr    0x800b9810
#define ios_alloc_addr          0x800b9b44
#define ios_free_addr           0x800b9b48
#define ramSetSize_addr         0x80041d7c
#define xlHeapTake_addr         0x800810f8
#define heap_size_hook_addr     0x8008A164
#define reset_flag_addr         0x8025D0EC
#define gSystem_ptr_addr        0x8020f1f8
#define N64_DRAM_SIZE           0x00800000
#define cpuMapObject_addr       0x8003c614
#define xlObjectMake_addr       0x800821f4
#define cpuSetDevicePut_addr    0x8003c7c8
#define cpuSetDeviceGet_addr    0x8003c7b0
#define xlHeapFree_addr         0x8008136c
#define OSCreateThread_addr     0x800929f4
#define OSResumeThread_addr     0x8009305c
#define OSSuspendThread_addr    0x800932f4
#elif VC_VERSION == NACE
#define init_hook_addr          0x800078E8
#define ios_openasync_addr      0x800b8858
#define ios_open_addr           0x800b8970
#define ios_closeasync_addr     0x800b8a90
#define ios_close_addr          0x800b8b50
#define ios_readasync_addr      0x800b8bf8
#define ios_read_addr           0x800b8cf8
#define ios_writeasync_addr     0x800b8e00
#define ios_write_addr          0x800b8f00
#define ios_seekasync_addr      0x800b9008
#define ios_seek_addr           0x800b90e8
#define ios_ioctlasync_addr     0x800b91d0
#define ios_ioctl_addr          0x800b9308
#define ios_ioctlvasync_addr    0x800b9574
#define ios_ioctlv_addr         0x800b9658
#define ios_create_heap_addr    0x800b981c
#define ios_alloc_addr          0x800b9b50
#define ios_free_addr           0x800b9b54
#define ramSetSize_addr         0x80041d98
#define xlHeapTake_addr         0x80081104
#define heap_size_hook_addr     0x8008a170
#define reset_flag_addr         0x8025D1EC
#define gSystem_ptr_addr        0x8020f2f8
#define N64_DRAM_SIZE           0x00800000
#define cpuMapObject_addr       0x8003c630
#define xlObjectMake_addr       0x80082200
#define cpuSetDevicePut_addr    0x8003c7e4
#define cpuSetDeviceGet_addr    0x8003c7cc
#define xlHeapFree_addr         0x8008136c
#define OSCreateThread_addr     0x80092a00
#define OSResumeThread_addr     0x80093068
#define OSSuspendThread_addr    0x80093300
#elif VC_VERSION == NARJ
#define ios_openasync_addr      0x800c5430
#define ios_open_addr           0x800c5548
#define ios_closeasync_addr     0x800c5668
#define ios_close_addr          0x800c5728
#define ios_readasync_addr      0x800c57d0
#define ios_read_addr           0x800c58d0
#define ios_writeasync_addr     0x800c59d8
#define ios_write_addr          0x800c5ad8
#define ios_seekasync_addr      0x800c5be0
#define ios_seek_addr           0x800c5cc0
#define ios_ioctlasync_addr     0x800c5da8
#define ios_ioctl_addr          0x800c5ee0
#define ios_ioctlvasync_addr    0x800c614c
#define ios_ioctlv_addr         0x800c6230
#define ios_create_heap_addr    0x800c6608
#define ios_alloc_addr          0x800c693c
#define ios_free_addr           0x800c6940
#define ramSetSize_addr         0x8005083c
#define xlHeapTake_addr         0x800887e0
#define reset_flag_addr         0x80200830
#define gSystem_ptr_addr        0x80200638
#define N64_DRAM_SIZE           0x00800000
#define cpuSetDevicePut_addr    0x8004B694
#define cpuSetDeviceGet_addr    0x8004B67C
#define xlObjectMake_addr       0x8008979C
#define cpuMapObject_addr       0x8004B27C
#define xlHeapFree_addr         0x80088A60
#define OSCreateThread_addr     0x8009b2d8
#define OSResumeThread_addr     0x8009b948
#define OSSuspendThread_addr    0x8009bbe0
#elif VC_VERSION == NARE
#define ios_openasync_addr      0x800c4dec
#define ios_open_addr           0x800c4f04
#define ios_closeasync_addr     0x800c5024
#define ios_close_addr          0x800c50e4
#define ios_readasync_addr      0x800c518c
#define ios_read_addr           0x800c528c
#define ios_writeasync_addr     0x800c5394
#define ios_write_addr          0x800c5494
#define ios_seekasync_addr      0x800c559c
#define ios_seek_addr           0x800c567c
#define ios_ioctlasync_addr     0x800c5764
#define ios_ioctl_addr          0x800c589c
#define ios_ioctlvasync_addr    0x800c5b08
#define ios_ioctlv_addr         0x800c5bec
#define ios_create_heap_addr    0x800c5fc4
#define ios_alloc_addr          0x800c62f8
#define ios_free_addr           0x800c62fc
#define ramSetSize_addr         0x800507c8
#define xlHeapTake_addr         0x80088790
#define reset_flag_addr         0x801FBA28
#define gSystem_ptr_addr        0x801fb838
#define N64_DRAM_SIZE           0x00800000
#define cpuMapObject_addr       0x8004b208
#define xlObjectMake_addr       0x8008974c
#define cpuSetDevicePut_addr    0x8004b620
#define cpuSetDeviceGet_addr    0x8004b608
#define xlHeapFree_addr         0x80088a10
#define OSCreateThread_addr     0x8009afbc
#define OSResumeThread_addr     0x8009b624
#define OSSuspendThread_addr    0x8009b8bc
#elif VC_VERSION == NAEJ
#define init_hook_addr          0x800079bc
#define ios_openasync_addr      0x800bf5ac
#define ios_open_addr           0x800bf6c4
#define ios_closeasync_addr     0x800bf7e4
#define ios_close_addr          0x800bf8a4
#define ios_readasync_addr      0x800bf94c
#define ios_read_addr           0x800c1e08
#define ios_writeasync_addr     0x800bfb54
#define ios_write_addr          0x800c1ecc
#define ios_seekasync_addr      0x800bfd5c
#define ios_seek_addr           0x800c1d78
#define ios_ioctlasync_addr     0x800bff24
#define ios_ioctl_addr          0x800c005c
#define ios_ioctlvasync_addr    0x800c02c8
#define ios_ioctlv_addr         0x800c03ac
#define ios_create_heap_addr    0x800c0570
#define ios_alloc_addr          0x800c08a4
#define ios_free_addr           0x800c08a8
#define ramSetSize_addr         0x80041754
#define xlHeapTake_addr         0x80085f48
#define heap_size_hook_addr     0x8008f778
#define reset_flag_addr         0x80246C7C
#define gSystem_ptr_addr        0x80246B64
#define N64_DRAM_SIZE           0x00800000
#define cpuMapObject_addr       0x8003c58c
#define xlObjectMake_addr       0x8008703c
#define cpuSetDevicePut_addr    0x8003c750
#define cpuSetDeviceGet_addr    0x8003c738
#define xlHeapFree_addr         0x800861b4
#define OSCreateThread_addr     0x80098154
#define OSResumeThread_addr     0x800987bc
#define OSSuspendThread_addr    0x80098a54
#elif VC_VERSION == NAEE
#define init_hook_addr          0x800079bc
#define ios_openasync_addr      0x800bf620
#define ios_open_addr           0x800bf738
#define ios_closeasync_addr     0x800bf858
#define ios_close_addr          0x800bf918
#define ios_readasync_addr      0x800bf9c0
#define ios_read_addr           0x800c1e7c
#define ios_writeasync_addr     0x800bfbc8
#define ios_write_addr          0x800c1f40
#define ios_seekasync_addr      0x800bfdd0
#define ios_seek_addr           0x800c1dec
#define ios_ioctlasync_addr     0x800bff98
#define ios_ioctl_addr          0x800c00d0
#define ios_ioctlvasync_addr    0x800c033c
#define ios_ioctlv_addr         0x800c0420
#define ios_create_heap_addr    0x800c05e4
#define ios_alloc_addr          0x800c0918
#define ios_free_addr           0x800c091c
#define ramSetSize_addr         0x80041754
#define xlHeapTake_addr         0x80085fbc
#define heap_size_hook_addr     0x8008f7ec
#define reset_flag_addr         0x80246dbc
#define gSystem_ptr_addr        0x80246ca4
#define N64_DRAM_SIZE           0x00800000
#define cpuMapObject_addr       0x8003c58c
#define xlObjectMake_addr       0x800870b0
#define cpuSetDevicePut_addr    0x8003c750
#define cpuSetDeviceGet_addr    0x8003c738
#define xlHeapFree_addr         0x80086228
#define OSCreateThread_addr     0x800981c8
#define OSResumeThread_addr     0x80098830
#define OSSuspendThread_addr    0x80098ac8
#endif

#define cur_thread_addr         0x800000C0
#define ex_handlers_addr        0x80003000
#define title_id_addr           0x80003180
#define ios_heap_addr           0x933e8000

typedef int     (*ios_create_heap_t)(void *heap, size_t size);
typedef void*   (*ios_alloc_t)(int hid, size_t size, size_t page_size);
typedef bool    (*ios_free_t)(int hid, void *ptr);
typedef int     (*ios_openasync_t)(const char *file, int mode, void *callback, void *callback_data);
typedef int     (*ios_open_t)(const char *file, int mode);
typedef int     (*ios_closeasync_t)(int fd, void *callback, void *callback_data);
typedef int     (*ios_close_t)(int fd);
typedef int     (*ios_readasync_t)(int fd, void *data, size_t len, void *callback, void *callback_data);
typedef int     (*ios_read_t)(int fd, void *data, size_t len);
typedef int     (*ios_writeasync_t)(int fd, void *data, size_t len, void *callback, void *callback_data);
typedef int     (*ios_write_t)(int fd, void *data, size_t len);
typedef int     (*ios_seekasync_t)(int fd, int where, int whence, void *callback, void *callback_data);
typedef int     (*ios_seek_t)(int fd, int where, int whence);
typedef int     (*ios_ioctlasync_t)(int fd, int ioctl, void *buffer_in, size_t size_in, void *buffer_io, size_t size_out, void *callback, void *callback_data);
typedef int     (*ios_ioctl_t)(int fd, int ioctl, void *buffer_in, size_t size_in, void *buffer_io, size_t size_out);
typedef int     (*ios_ioctlvasync_t)(int fd, int ioctl, int cnt_in, int cnt_io, void *argv, void *callback, void *callback_data);
typedef int     (*ios_ioctlv_t)(int fd, int ioctl, int cnt_in, int cnt_io, void *argv);
typedef bool    (*ramSetSize_t)(void **dest,uint32_t size);
typedef bool    (*xlHeapTake_t)(void **dest, uint32_t size);
typedef bool    (*xlHeapFree_t)(void *ptr);
typedef int     (*xlObjectMake_t)(void **obj, void *parent, class_type_t *class);
typedef int     (*cpuMapObject_t)(gClassCPU_t *cpu, void *dev_p, uint32_t address_start, uint32_t address_end, uint32_t param_5);
typedef int     (*cpuSetDevicePut_t)(gClassCPU_t *cpu, cpu_dev_t *dev,void *sb,void *sh,void *sw,void *sd);
typedef int     (*cpuSetDeviceGet_t)(gClassCPU_t *cpu, cpu_dev_t *dev,void *lb,void *lh,void *lw,void *ld);
typedef void    *(*OSThreadEntry_t)(void *arg);
typedef void    (*OSCreateThread_t)(thread_context_t *thread, OSThreadEntry_t entry, void *arg, void *stack, size_t stack_size, int pri, int detached);
typedef void    (*OSResumeThread_t)(thread_context_t *thread);
typedef void    (*OSSuspendThread_t)(thread_context_t *thread);
typedef void    (*ex_handler_t)(enum ppc_exception);

#define title_id        (*(uint32_t*)           title_id_addr)
#define reset_flag      (*(uint32_t*)           reset_flag_addr)

#define cur_thread      (*(thread_context_t**)  cur_thread_addr)
#define ex_handlers     ((ex_handler_t*)        ex_handlers_addr)
#define gSystem         (*(gClassSystem_t**)    gSystem_ptr_addr)
#define n64_cpu         gSystem->cpu    

#define ios_openasync   ((ios_openasync_t)      ios_openasync_addr)
#define ios_open        ((ios_open_t)           ios_open_addr)
#define ios_closeasync  ((ios_closeasync_t)     ios_closeasync_addr)
#define ios_close       ((ios_close_t)          ios_close_addr)
#define ios_readasync   ((ios_readasync_t)      ios_readasync_addr)
#define ios_read        ((ios_read_t)           ios_read_addr)
#define ios_writeasync  ((ios_writeasync_t)     ios_writeasync_addr)
#define ios_write       ((ios_write_t)          ios_write_addr)
#define ios_seekasync   ((ios_seekasync_t)      ios_seekasync_addr)
#define ios_seek        ((ios_seek_t)           ios_seek_addr)
#define ios_ioctlasync  ((ios_ioctlasync_t)     ios_ioctlasync_addr)
#define ios_ioctl       ((ios_ioctl_t)          ios_ioctl_addr)
#define ios_ioctlvasync ((ios_ioctlvasync_t)    ios_ioctlvasync_addr)
#define ios_ioctlv      ((ios_ioctlv_t)         ios_ioctlv_addr)

#define ios_create_heap ((ios_create_heap_t)    ios_create_heap_addr)
#define ios_alloc       ((ios_alloc_t)          ios_alloc_addr)
#define ios_free        ((ios_free_t)           ios_free_addr)

#define ramSetSize      ((ramSetSize_t)         ramSetSize_addr)
#define xlHeapTake      ((xlHeapTake_t)         xlHeapTake_addr)
#define xlHeapFree      ((xlHeapFree_t)         xlHeapFree_addr)
#define xlObjectMake    ((xlObjectMake_t)       xlObjectMake_addr)
#define cpuMapObject    ((cpuMapObject_t)       cpuMapObject_addr)
#define cpuSetDevicePut ((cpuSetDevicePut_t)    cpuSetDevicePut_addr)
#define cpuSetDeviceGet ((cpuSetDeviceGet_t)    cpuSetDeviceGet_addr)
#define OSCreateThread  ((OSCreateThread_t)     OSCreateThread_addr)
#define OSResumeThread  ((OSResumeThread_t)     OSResumeThread_addr)
#define OSSuspendThread ((OSSuspendThread_t)    OSSuspendThread_addr)

#define allocMEM2(ptr, size)    xlHeapTake((void**)(ptr), (0x70000000 | (size)))

#endif
