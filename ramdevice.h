#ifndef RAMDEVICE_H
#define RAMDEVICE_H

#define RB_SECTOR_SIZE 512

extern int ramdevice_initial(void);
extern void ramdevice_clean(void);
extern void ramdevice_write(sector_t sector_off, u8 *buffer, unsigned int sectors);
extern void ramdevice_read(sector_t sector_off, u8 *buffer, unsigned int sectors);
#endif
