#ifndef FAT_DATA_H
#define FAT_DATA_H

uint8_t BootSector[] = {
	0xEB, 0x3C, 0x90,		/* code to jump to the bootstrap code */
	'm', 'k', 'd', 'o', 's', 'f', 's', 0x00, /* OEM ID */
	WBVAL(BYTES_PER_SECTOR),	/* bytes per sector */
	SECTORS_PER_CLUSTER,		/* sectors per cluster */
	WBVAL(RESERVED_SECTORS),	/* # of reserved sectors (1 boot sector) */
	FAT_COPIES,			/* FAT copies (2) */
	WBVAL(ROOT_ENTRIES),		/* root entries (512) */
	WBVAL(SECTOR_COUNT),		/* total number of sectors */
	0xF8,				/* media descriptor (0xF8 = Fixed disk) */
	0x01, 0x00,			/* sectors per FAT (1) */
	0x20, 0x00,			/* sectors per track (32) */
	0x40, 0x00,			/* number of heads (64) */
	0x00, 0x00, 0x00, 0x00,		/* hidden sectors (0) */
	0x00, 0x00, 0x00, 0x00,		/* large number of sectors (0) */
	0x00,				/* drive number (0) */
	0x00,				/* reserved */
	0x29,				/* extended boot signature */
	0x69, 0x17, 0xAD, 0x53,		/* volume serial number */
	'R', 'A', 'M', 'D', 'I', 'S', 'K', ' ', ' ', ' ', ' ', /* volume label */
	'F', 'A', 'T', '1', '2', ' ', ' ', ' '	/* filesystem type */
};

uint8_t FatSector[] = {
	0xF8, 0xFF, 0xFF, 0x00, 0x40, 0x00, 0x05, 0x60, 0x00, 0x07, 0x80, 0x00,
	0x09, 0xA0, 0x00, 0x0B, 0xC0, 0x00, 0x0D, 0xE0, 0x00, 0x0F, 0x00, 0x01,
	0x11, 0x20, 0x01, 0x13, 0x40, 0x01, 0x15, 0x60, 0x01, 0x17, 0x80, 0x01,
	0x19, 0xA0, 0x01, 0x1B, 0xC0, 0x01, 0x1D, 0xE0, 0x01, 0x1F, 0x00, 0x02,
	0x21, 0x20, 0x02, 0x23, 0x40, 0x02, 0x25, 0x60, 0x02, 0x27, 0x80, 0x02,
	0x29, 0xA0, 0x02, 0x2B, 0xC0, 0x02, 0x2D, 0xE0, 0x02, 0x2F, 0x00, 0x03,
	0x31, 0x20, 0x03, 0x33, 0x40, 0x03, 0x35, 0x60, 0x03, 0x37, 0x80, 0x03,
	0x39, 0xA0, 0x03, 0x3B, 0xC0, 0x03, 0x3D, 0xE0, 0x03, 0x3F, 0x00, 0x04,
	0x41, 0x20, 0x04, 0x43, 0x40, 0x04, 0x45, 0x60, 0x04, 0x47, 0x80, 0x04,
	0x49, 0xA0, 0x04, 0x4B, 0xC0, 0x04, 0x4D, 0xE0, 0x04, 0x4F, 0x00, 0x05,
	0x51, 0x20, 0x05, 0x53, 0x40, 0x05, 0x55, 0x60, 0x05, 0x57, 0x80, 0x05,
	0x59, 0xA0, 0x05, 0x5B, 0xC0, 0x05, 0x5D, 0xE0, 0x05, 0x5F, 0x00, 0x06,
	0x61, 0x20, 0x06, 0x63, 0x40, 0x06, 0x65, 0x60, 0x06, 0x67, 0x80, 0x06,
	0x69, 0xA0, 0x06, 0x6B, 0xC0, 0x06, 0x6D, 0xE0, 0x06, 0x6F, 0x00, 0x07,
	0x71, 0x20, 0x07, 0x73, 0x40, 0x07, 0x75, 0x60, 0x07, 0x77, 0x80, 0x07,
	0x79, 0xA0, 0x07, 0x7B, 0xC0, 0x07, 0x7D, 0xE0, 0x07, 0x7F, 0x00, 0x08,
	0x81, 0x20, 0x08, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

uint8_t DirSector[] = {
	/* long filename entry */
	0x41,						/* sequence number */
	WBVAL('p'), WBVAL('a'), WBVAL('s'), WBVAL('s'), WBVAL('l'), /* five name characters in UTF-16 */
	0x0F,						/* attributes */
	0x00,						/* type */
	0x00,						/* checksum of DOS filename (computed in ramdisk_init) */
	WBVAL('s'), WBVAL('t'), WBVAL('.'), WBVAL('t'), WBVAL('x'), WBVAL('t'),	/* six name characters in UTF-16 */
	0x00, 0x00,					/* first cluster */
	WBVAL(0), WBVAL(0),				/* two name characters in UTF-16 */
	/* actual entry */
	'p', 'a', 's', 's', 'l', 's', 't', ' ',		/* filename */
	't', 'x', 't',					/* extension */
	0x20,						/* attribute byte */
	0x00,						/* reserved for Windows NT */
	0x00,						/* creation millisecond */
	0xCE, 0x01,					/* creation time */
	0x86, 0x41,					/* creation date */
	0x86, 0x41,					/* last access date */
	0x00, 0x00,					/* reserved for FAT32 */
	0xCE, 0x01,					/* last write time */
	0x86, 0x41,					/* last write date */
	WBVAL(FILEDATA_START_CLUSTER),			/* start cluster */
	QBVAL(FILEDATA_SECTOR_COUNT * SECTOR_SIZE)	/* file size in bytes */
};


#endif /* FAT_DATA_H */
