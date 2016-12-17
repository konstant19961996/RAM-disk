#include <linux/types.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/errno.h>

#include "ramdevice.h"
#include "part.h"

#define RB_DEVICE_SIZE 1024 
static u8 *dev_data;

int ramdevice_initial(void)
{
	dev_data = vmalloc(RB_DEVICE_SIZE * RB_SECTOR_SIZE);
	if (dev_data == NULL)
		return -ENOMEM;
	copy_mbr_to_br(dev_data);
	return RB_DEVICE_SIZE;
}

void ramdevice_clean(void)
{
	vfree(dev_data);
}

void ramdevice_write(sector_t sector_off, u8 *buffer, unsigned int sectors)
{
	memcpy(dev_data + sector_off * RB_SECTOR_SIZE, buffer,
		sectors * RB_SECTOR_SIZE);
}
void ramdevice_read(sector_t sector_off, u8 *buffer, unsigned int sectors)
{
	memcpy(buffer, dev_data + sector_off * RB_SECTOR_SIZE,
		sectors * RB_SECTOR_SIZE);
}
