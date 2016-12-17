#include <linux/string.h>

#include "part.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

#define SECTOR_SIZE 512
#define MBR_SIZE SECTOR_SIZE
#define MBR_DISK_SIGNATURE_OFFSET 440
#define MBR_DISK_SIGNATURE_SIZE 4
#define PARTITION_TABLE_OFFSET 446
#define PARTITION_ENTRY_SIZE 16 
#define PARTITION_TABLE_SIZE 64 
#define MBR_SIGNATURE_OFFSET 510
#define MBR_SIGNATURE_SIZE 2
#define MBR_SIGNATURE 0xAA55
#define BR_SIZE SECTOR_SIZE
#define BR_SIGNATURE_OFFSET 510
#define BR_SIGNATURE_SIZE 2
#define BR_SIGNATURE 0xAA55

typedef struct
{
	unsigned char boottype; 
	unsigned char starthead;
	unsigned char startsec:6;
	unsigned char startcyl_hi:2;
	unsigned char startcyl;
	unsigned char parttype;
	unsigned char endhead;
	unsigned char endsec:6;
	unsigned char endcyl_hi:2;
	unsigned char endcyl;
	unsigned int abssecstart;
	unsigned int secpart;
} PartEntry;

typedef PartEntry PartTable[4];

static PartTable def_part_table =
{
	{
		boottype: 0x00,
		starthead: 0x00,
		startsec: 0x2,
		startcyl: 0x00,
		parttype: 0x83,
		endhead: 0x00,
		endsec: 0x20,
		endcyl: 0x09,
		abssecstart: 0x00000001,
		secpart: 0x0000013F
	},
	{
		boottype: 0x00,
		starthead: 0x00,
		startsec: 0x1,
		startcyl: 0x0A, 
		parttype: 0x05,
		endhead: 0x00,
		endsec: 0x20,
		endcyl: 0x13,
		abssecstart: 0x00000140,
		secpart: 0x00000140
	},
	{
		boottype: 0x00,
		starthead: 0x00,
		startsec: 0x1,
		startcyl: 0x14,
		parttype: 0x83,
		endhead: 0x00,
		endsec: 0x20,
		endcyl: 0x1F,
		abssecstart: 0x00000280,
		secpart: 0x00000180
	},
	{
	}
};
static unsigned int def_log_part_br_cyl[] = {0x0A, 0x0E, 0x12};
static const PartTable def_log_part_table[] =
{
	{
		{
			boottype: 0x00,
			starthead: 0x00,
			startsec: 0x2,
			startcyl: 0x0A,
			parttype: 0x83,
			endhead: 0x00,
			endsec: 0x20,
			endcyl: 0x0D,
			abssecstart: 0x00000001,
			secpart: 0x0000007F
		},
		{
			boottype: 0x00,
			starthead: 0x00,
			startsec: 0x1,
			startcyl: 0x0E,
			parttype: 0x05,
			endhead: 0x00,
			endsec: 0x20,
			endcyl: 0x11,
			abssecstart: 0x00000080,
			secpart: 0x00000080
		},
	},
	{
		{
			boottype: 0x00,
			starthead: 0x00,
			startsec: 0x2,
			startcyl: 0x0E,
			parttype: 0x83,
			endhead: 0x00,
			endsec: 0x20,
			endcyl: 0x11,
			abssecstart: 0x00000001,
			secpart: 0x0000007F
		},
		{
			boottype: 0x00,
			starthead: 0x00,
			startsec: 0x1,
			startcyl: 0x12,
			parttype: 0x05,
			endhead: 0x00,
			endsec: 0x20,
			endcyl: 0x13,
			abssecstart: 0x00000100,
			secpart: 0x00000040
		},
	},
	{
		{
			boottype: 0x00,
			starthead: 0x00,
			startsec: 0x2,
			startcyl: 0x12,
			parttype: 0x83,
			endhead: 0x00,
			endsec: 0x20,
			endcyl: 0x13,
			abssecstart: 0x00000001,
			secpart: 0x0000003F
		},
	}
};

static void copymbr(u8 *disk)
{
	memset(disk, 0x0, MBR_SIZE);
	*(unsigned long *)(disk + MBR_DISK_SIGNATURE_OFFSET) = 0x36E5756D;
	memcpy(disk + PARTITION_TABLE_OFFSET, &def_part_table, PARTITION_TABLE_SIZE);
	*(unsigned short *)(disk + MBR_SIGNATURE_OFFSET) = MBR_SIGNATURE;
}
static void copybr(u8 *disk, int start_cylinder, const PartTable *part_table)
{
	disk += (start_cylinder * 32  * SECTOR_SIZE);
	memset(disk, 0x0, BR_SIZE);
	memcpy(disk + PARTITION_TABLE_OFFSET, part_table,
		PARTITION_TABLE_SIZE);
	*(unsigned short *)(disk + BR_SIGNATURE_OFFSET) = BR_SIGNATURE;
}
void copy_mbr_ti_br(u8 *disk)
{
	int i;

	copymbr(disk);
	for (i = 0; i < ARRAY_SIZE(def_log_part_table); i++)
	{
		copybr(disk, def_log_part_br_cyl[i], &def_log_part_table[i]);
	}
}
