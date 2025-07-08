/*
 */

#ifndef __TRI_NAND_DRV_H__
#define __TRI_NAND_DRV_H__

#include "../tri_fmc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* 非ECC0模式下缺省OOB长度 */
#define TRI_NFC_ECC_DEFAULT_OOB_SIZE     32

/* page0 支持最大有效数据长度 */
#define TRI_NFC_MAX_PAGE0_LEN            0x410

/* 中断使能掩码 */
#define TRI_NFC_INTERRUPT_MASK           0x7FF

/* 发给NAND Flash的地址周期数 */
#define TRI_NFC_RW_ADDR_CYCLE            0x5
#define TRI_NFC_ERASE_ADDR_CYCLE         0x3
#define TRI_NFC_READID_ADDR_CYCLE        0x1

/* nandc支持最大DMA buffer长度 */
#define TRI_NFC_MAX_DMA_BUF_LEN          0x1000

#define TRI_NAND_READ_ID_LEN             0x5

typedef tri_int32(*tri_nand_init_cb)(struct tri_nand_host *host);
typedef tri_int32(*tri_nand_write_cb)(struct tri_nand_host *host,
	tri_uchar8 *buf, tri_uint32 dst_addr);
typedef tri_int32(*tri_nand_read_cb)(struct tri_nand_host *host,
	tri_uchar8 *buf, tri_uint32 src_addr);
typedef tri_int32(*tri_nand_erase_cb)(struct tri_nand_host *host,
	tri_uint32 dst_addr);
typedef tri_void(*tri_nand_read_id_cb)(struct tri_nand_host *host,
	tri_uchar8 *buf, tri_uchar8 id_len);
typedef tri_int32(*tri_nand_read_oob_cb)(struct tri_nand_host *host,
	tri_uchar8 *buf, tri_uint32 src_addr);
typedef tri_int32(*tri_nand_write_oob_cb)(struct tri_nand_host *host,
	tri_uchar8 *buf, tri_uint32 dst_addr);
typedef tri_int32(*tri_nand_reset_cb)(tri_void);

enum tri_nand_addr_num_e {
	TRI_NAND_ADDR_NUM_5CYCLE_E = 0x0,
	TRI_NAND_ADDR_NUM_4CYCLE_E = 0x1,
};

enum tri_nand_op_mode_e {
	TRI_NAND_OP_MODE_BOOT_E = 0x0,
	TRI_NAND_OP_MODE_NORMAL_E = 0x1,
};

enum tri_nand_bus_width_e {
	TRI_NAND_BUS_WIDTH_8BIT_E = 0x0,
	TRI_NAND_BUS_WIDTH_16BIT_E = 0x1,
};

enum tri_page_size_e {
	TRI_NAND_PAGE_2KB_E = 0x1,
	TRI_NAND_PAGE_4KB_E = 0x2,
	TRI_NAND_PAGE_8KB_E = 0x3
};

#define TRI_NAND_PAGE_2KB  0x800
#define TRI_NAND_PAGE_4KB  0x1000
#define TRI_NAND_PAGE_8KB  0x2000

struct tri_nfc_reg_s {
	tri_uint32 nfc_con;       /* (0x00) -NANDC配置寄存器 */
	tri_uint32 nfc_pwidth;    /* (0x04) -读写脉冲宽度配置寄存器 */
	tri_uint32 nfc_opidle;    /* (0x08) -操作间隔配置寄存器 */
	tri_uint32 nfc_cmd;       /* (0x0c) -命令字配置寄存器 */
	tri_uint32 nfc_addrl;     /* (0x10) -低位地址配置寄存器 */
	tri_uint32 nfc_addrh;     /* (0x14) -高位地址配置寄存器 */
	tri_uint32 nfc_data_num;  /* (0x18) -读写数据数目配置寄存器 */
	tri_uint32 nfc_op;        /* (0x1c) -操作寄存器 */
	tri_uint32 nfc_status;    /* (0x20) -状态寄存器 */
	tri_uint32 nfc_inten;     /* (0x24) -中断使能寄存器 */
	tri_uint32 nfc_ints;      /* (0x28) -中断状态寄存器 */
	tri_uint32 nfc_intcrl;    /* (0x2c) -中断清除寄存器 */
	tri_uint32 nfc_lock;      /* (0x30) -锁地址配置寄存器 */
	tri_uint32 nfc_lock_sa0;  /* (0x34) -锁起始地址0配置寄存器 */
	tri_uint32 nfc_lock_sa1;  /* (0x38) -锁起始地址1配置寄存器 */
	tri_uint32 nfc_lock_sa2;  /* (0x3c) -锁起始地址2配置寄存器 */
	tri_uint32 nfc_lock_sa3;  /* (0x40) -锁起始地址3配置寄存器 */
	tri_uint32 nfc_lock_ea0;  /* (0x44) -锁结束地址0配置寄存器 */
	tri_uint32 nfc_lock_ea1;  /* (0x48) -锁结束地址1配置寄存器 */
	tri_uint32 nfc_lock_ea2;  /* (0x4c) -锁结束地址2配置寄存器 */
	tri_uint32 nfc_lock_ea3;  /* (0x50) -锁结束地址3配置寄存器 */

	tri_uint32 nfc_expcmd;    /* (0x54) -扩展页命令寄存器 */
	tri_uint32 nfc_exbcmd;    /* (0x58) -扩展块命令寄存器 */

	tri_uint32 nfc_ecc_test;  /* (0x5c) -ECC测试寄存器 */
	tri_uint32 nfc_dma_ctrl;  /* (0x60) -DMA控制寄存器 */
	tri_uint32 nfc_baddr_d;   /* (0x64) -传送数据区的基地址寄存器 */
	tri_uint32 nfc_baddr_oob; /* (0x68) -传送OOB区的基地址寄存器 */
	tri_uint32 nfc_dma_len;   /* (0x6c) -传送长度寄存器 */
	tri_uint32 nfc_op_para;   /* (0x70) -操作参数寄存器 */
	tri_uint32 nfc_version;   /* (0x74) -nandc版本寄存器 */

	tri_uint32 nfc_reserved1[3];  /* (0x78-0x80) -保留 */
	 /* (0x84) -DMA读写nandflash的数据段ID寄存器 */
	tri_uint32 nfc_segment_id;
	tri_uint32 nfc_reserved2[2];  /* (0x88-0x8c) -保留 */
	tri_uint32 nfc_fifo_empty;    /* (0x90) -内部fifo状态寄存器 */

	tri_uint32 nfc_boot_set;      /* (0x94) -boot参数设定寄存器 */
	tri_uint32 nfc_reserved3;     /* (0x98) -保留 */
	/* (0x9c) -nandc 的低功耗控制寄存器 */
	tri_uint32 nfc_lp_ctrl;
	/* (0xa0) -nandflash 第一次buffer纠错统计寄存器0 */
	tri_uint32 nfc_err_num0_buf0;
	/* (0xa4) -nandflash 第一次buffer纠错统计寄存器1 */
	tri_uint32 nfc_err_num1_buf0;
	/* (0xa8) -nandflash 第二次buffer纠错统计寄存器0 */
	tri_uint32 nfc_err_num0_buf1;
	/* (0xac) -nandflash 第二次buffer纠错统计寄存器1 */
	tri_uint32 nfc_err_num1_buf1;
	/* (0xb0) -nandflash 的ready_busy模式寄存器 */
	tri_uint32 nfc_rb_mode;
	tri_uint32 nfc_baddr_d1;    /* (0xb4) -传送数据区的基地址寄存器1 */
	tri_uint32 nfc_baddr_d2;    /* (0xb8) -传送数据区的基地址寄存器2 */
	tri_uint32 nfc_baddr_d3;    /* (0xbc) -传送数据区的基地址寄存器3 */

	tri_uint32 nfc_reserved4;   /* (0xc0) -保留 */

	tri_uint32 nfc_boot_cfg;    /* (0xc4) -nandflash 的boot配置寄存器 */
	tri_uint32 nfc_oob_sel;     /* (0xc8) -16bit ECC，OOB区长度选择寄存器 */
	tri_uint32 nfc_mem_ctrl;    /* (0xcc) -例化mem管理 */
	tri_uint32 nfc_sync_timing; /* (0xd0) -同步NAND读写时序参数 */
	tri_uint32 nfc_dll_ctrl;    /* (0xd4) -DLL模块控制信息 */
	tri_uint32 nfc_dll_status;  /* (0xd8) -DLL模块状态信息 */
};

union tri_nfc_con_u {
	tri_uint32 value;
	struct {
		tri_uint32 op_mode: 1;     /* [0]-nandc操作模式 */
		tri_uint32 page_type: 3;   /* [1:3]-nandflash页大小 */
		tri_uint32 bus_width: 1;   /* [4]-nandflash数据线宽度 */
		tri_uint32 reserved: 2;    /* [5:6]-保留 */
		tri_uint32 cs_ctrl: 1;     /* [7]-片选控制 */
		tri_uint32 rb_sel: 1;      /* [8]-ready/busy信号控制 */
		tri_uint32 ecc_type: 4;    /* [9:12]-ecc模式 */
		/* [13:31]-不同芯片有不同定义,这里全部reserved,由驱动去区分 */
		tri_uint32 reserved1: 19;
	} bits;
};

union tri_nfc_cmd_u {
	tri_uint32 value;
	struct {
		/* [0:7]  -controller发给nandflash的第一个命令 */
		tri_uint32 cmd1: 8;
		/* [8:15] -controller发给nandflash的第二个命令 */
		tri_uint32 cmd2: 8;
		tri_uint32 read_status_cmd: 8;  /* [16:23]-read status命令字 */
		tri_uint32 reserved: 8;         /* [24:31]-保留 */
	} bits;
};

union tri_nfc_addrl_u {
	tri_uint32 value;
	struct {
		/* [0:15]-页内地址，只在bus方式只读OOB时使用 */
		tri_uint32 page_inner_addr: 16;
		tri_uint32 page_addr: 16;       /* [16:31]-页地址 */
	} rw_bits;
	struct {
		tri_uint32 erase_addr: 24;      /* [0:23]-擦除操作时的块地址 */
		tri_uint32 reserved: 8;         /* [24:31]-保留 */
	} erase_bits;
};


/* NFC_ADDRH reg 读写时可能使用，擦除不使用 */

union tri_nfc_addrh_u {
	tri_uint32 value;
	struct {
		tri_uint32 addr_h: 8;     /* [0:7]-高8bit地址空间 */
		tri_uint32 reserved: 24;  /* [8:31]-保留 */
	} rw_bits;
};

/*
 * NFC_OP reg
 * [0]-读状态使能
 * [1]-读数据使能
 * [2]-等待ready/busy信号变高使能
 * [3]-发cmd2命令使能
 * [4]-向nandflash写数据使能
 * [5]-向nand写操作地址使能
 * [6]-发cms1命令使能
 * [7:8]-选择操作的nandflash器件，即片选
 * [9:11]-发给nandflash的地址周期数
 * [12]-对于toggle nand，0:非读ID操作，1:读ID操作
 * [13]-对同步nand，0:读写数据，1:读写寄存器
 * [14:31]-保留
 */
union tri_nfc_op_u {
	tri_uint32 value;
	struct {
		tri_uint32 read_status_en: 1;
		tri_uint32 read_data_en: 1;
		tri_uint32 wait_ready_en: 1;
		tri_uint32 cmd2_en: 1;
		tri_uint32 write_data_en: 1;
		tri_uint32 addr_en: 1;
		tri_uint32 cmd1_en: 1;
		tri_uint32 nf_cs: 2;
		tri_uint32 address_cycle: 3;
		tri_uint32 read_id_en: 1;
		tri_uint32 rw_reg_en: 1;
		tri_uint32 reserved: 18;
	} bits;
};

/*
 * NFC_STATUS reg
 * [0]-nandc当前操作状态
 * [1]-片选0器件的ready/busy信号状态
 * [2]-片选1器件的ready/busy信号状态
 * [3]-片选2器件的ready/busy信号状态
 * [4]-片选3器件的ready/busy信号状态
 * [5:7]-保留
 * [8:15]-从nandflash读回的nandflash 的status数据
 * [16:18]-nandc ecc模式，与CON相同，只读
 * [19]-randomizer状态，与CON相同个，只读
 * [20:31] -保留
 */
union tri_nfc_status_u {
	tri_uint32 value;
	struct {
		tri_uint32 nfc_ready: 1;
		tri_uint32 nf0_ready: 1;
		tri_uint32 nf1_ready: 1;
		tri_uint32 nf2_ready: 1;
		tri_uint32 nf3_ready: 1;
		tri_uint32 reserved: 3;
		tri_uint32 nf_status: 8;
		tri_uint32 nf_ecc_type: 3;
		tri_uint32 nf_randomizer_en: 1;
		tri_uint32 reserved1: 12;
	} bits;
};

/*
 * NFC_INTS reg
 * [0]-NANDC本次操作结束中断
 * [1]-片选0对应的ready/busy信号由低变高中断
 * [2]-片选1对应的ready/busy信号由低变高中断
 * [3]-片选2对应的ready/busy信号由低变高中断
 * [4]-片选3对应的ready/busy信号由低变高中断
 * [5]-产生可纠正的错误中断
 * [6]-产生不可纠正的错误中断
 * [7]-NANDC读写flash器件cpu读写nandc buffer错误中断
 * [8]-对lock地址进行写操作错误中断
 * [9]-DMA传输完成中断
 * [10]-DMA传输中线出现错误中断
 * [11:31]-保留
 */
union tri_nfc_ints_u {
	tri_uint32 value;
	struct {
		tri_uint32 op_done: 1;
		tri_uint32 cs0_done: 1;
		tri_uint32 cs1_done: 1;
		tri_uint32 cs2_done: 1;
		tri_uint32 cs3_done: 1;
		tri_uint32 err_valid: 1;
		tri_uint32 err_invalid: 1;
		tri_uint32 ahb_op: 1;
		tri_uint32 wr_lock: 1;
		tri_uint32 dma_done: 1;
		tri_uint32 dma_err: 1;
		tri_uint32 reserved: 21;
	} bits;
};

union tri_nfc_dma_ctrl_u {
	tri_uint32 value;
	struct {
		tri_uint32 dma_start: 1;        /* [0]-启动dma操作 */
		tri_uint32 dma_wr_en: 1;        /* [1]-dma读写使能 */
		tri_uint32 reserved: 2;         /* [2:3]-保留 */
		tri_uint32 burst4_en: 1;        /* [4]-burst4使能 */
		tri_uint32 burst8_en: 1;        /* [5]-burst8使能 */
		tri_uint32 burst16_en: 1;       /* [6]-burst16使能 */
		tri_uint32 dma_addr_num: 1;     /* [7]-地址数 */
		tri_uint32 dma_nf_cs: 2;        /* [8:9]-选择dma片选 */
		tri_uint32 reserved1: 1;        /* [10]-保留 */
		tri_uint32 wr_cmd_disable: 1;   /* [11]-是否发出写命令 */
		tri_uint32 dma_rd_oob: 1;       /* [12]-是否只读OOB */
		tri_uint32 reserved2: 19;       /* [13:31]-保留 */
	} bits;
};

union tri_nfc_op_para_u {
	tri_uint32 value;
	struct {
		tri_uint32 data_rw_en: 1;   /* [0] -nandflash数据区读写 */
		tri_uint32 oob_rw_en: 1;    /* [1] -nandflash冗余区读写使能 */
		tri_uint32 data_edc_en: 1;  /* [2] -data区校验使能 */
		tri_uint32 oob_edc_en: 1;   /* [3] -oob区校验使能 */
		tri_uint32 reserved: 28;    /* [4:31] -保留 */
	} bits;
};


struct tri_nand_host {
	struct tri_nand_chip *chip;
	struct mtd_info *mtd;
	struct device *dev;
	/* 支持 ecc 1bit/4bit/8bit/24bit */
	enum tri_nand_ecc_type_e ecc_type;
	enum tri_page_size_e page_type; /* 支持页 2k/4k/8k */

	tri_uint32 dma;         /* dma 缓冲物理地址 */
	tri_uint32 dma_extern;  /* 8KB页时需要一个扩展的dma缓冲 */
	tri_uint32 dma_oob;     /* oob dma 物理地址 */

	tri_uchar8 *buffer;     /* dma 缓冲虚拟地址 */
	tri_uchar8 *buffer_extern; /* 8KB页时需要一个扩展的dma缓冲 */
	tri_uchar8 *oob_buffer;

	tri_void __iomem *nand_buffer; /* nandc 缓冲 */
};

/* bus方式只读OOB的情况下，包含OOB信息的sector的页内起始地址 */
struct tri_nand_oob_offset {
	tri_uint32 ecc_type;
	tri_uint32 page_type;
	tri_uint32 oob_sel;
	tri_uint32 oob_offset;
};

struct tri_nand_func_s {
	tri_nand_init_cb init;
	tri_nand_write_cb write;
	tri_nand_read_cb read;
	tri_nand_erase_cb erase;
	tri_nand_read_id_cb read_id;
	tri_nand_read_oob_cb read_oob;
	tri_nand_write_oob_cb write_oob;
	tri_nand_reset_cb reset;
};

tri_int32 tri_nand_drv_reset(tri_void);
tri_int32 tri_nand_drv_init(struct tri_nand_host *host);
tri_int32 tri_nand_drv_exit(struct tri_nand_host *host);
tri_int32 tri_nand_drv_pre_init(struct tri_nand_host *host);
tri_int32 tri_nand_drv_erase(struct tri_nand_host *host, tri_uint32 dst_addr);
tri_void tri_nand_drv_read_id(struct tri_nand_host *host,
	tri_uchar8 *buf, tri_uchar8 id_len);
tri_int32 tri_nand_drv_read(struct tri_nand_host *host,
	tri_uchar8 *buf, tri_uint32 dst_addr);
tri_int32 tri_nand_drv_read_oob(struct tri_nand_host *host,
	tri_uchar8 *buf, tri_uint32 dst_addr);
tri_int32 tri_nand_drv_write(struct tri_nand_host *host,
	tri_uchar8 *buf, tri_uint32 dst_addr);
tri_int32 tri_nand_drv_write_oob(struct tri_nand_host *host,
	tri_uchar8 *buf, tri_uint32 dst_addr);
tri_int32 tri_nand_drv_wp_check(tri_void);
tri_uint32 tri_nand_drv_is_nand_flash_type(tri_void);
tri_int32 tri_nand_drv_dma_init(struct tri_nand_host *host);

struct tri_nand_func_s *tri_nand_get_ops(tri_void);
struct tri_nand_func_s *tri_spi_nand_get_ops(tri_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __TRI_NAND_DRV_H__ */