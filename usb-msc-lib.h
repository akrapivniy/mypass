/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   usb-msc-lib.h
 * Author: umka
 *
 * Created on 7 июня 2016 г., 11:47
 */

#ifndef USB_MSC_LIB_H
#define USB_MSC_LIB_H

typedef struct _usbd_mass_storage usbd_mass_storage;

/* Definitions of Mass Storage Class from:
 *
 * (A) "Universal Serial Bus Mass Storage Class Bulk-Only Transport
 *      Revision 1.0"
 *
 * (B) "Universal Serial Bus Mass Storage Class Specification Overview
 *      Revision 1.0"
 */

/* (A) Table 4.5: Mass Storage Device Class Code */
#define USB_CLASS_MSC			0x08

/* (B) Table 2.1: Class Subclass Code */
#define USB_MSC_SUBCLASS_RBC		0x01
#define USB_MSC_SUBCLASS_ATAPI		0x02
#define USB_MSC_SUBCLASS_UFI		0x04
#define USB_MSC_SUBCLASS_SCSI		0x06
#define USB_MSC_SUBCLASS_LOCKABLE	0x07
#define USB_MSC_SUBCLASS_IEEE1667	0x08

/* (B) Table 3.1 Mass Storage Interface Class Control Protocol Codes */
#define USB_MSC_PROTOCOL_CBI		0x00
#define USB_MSC_PROTOCOL_CBI_ALT	0x01
#define USB_MSC_PROTOCOL_BBB		0x50

/* (B) Table 4.1 Mass Storage Request Codes */
#define USB_MSC_REQ_CODES_ADSC		0x00
#define USB_MSC_REQ_CODES_GET		0xFC
#define USB_MSC_REQ_CODES_PUT		0xFD
#define USB_MSC_REQ_CODES_GML		0xFE
#define USB_MSC_REQ_CODES_BOMSR		0xFF

/* (A) Table 3.1/3.2 Class-Specific Request Codes */
#define USB_MSC_REQ_BULK_ONLY_RESET	0xFF
#define USB_MSC_REQ_GET_MAX_LUN		0xFE

usbd_mass_storage *usb_msc_init(usbd_device *usbd_dev,
				 uint8_t ep_in, uint8_t ep_in_size,
				 uint8_t ep_out, uint8_t ep_out_size,
				 const char *vendor_id,
				 const char *product_id,
				 const char *product_revision_level,
				 const uint32_t block_count,
				 int (*read_block)(uint32_t lba, uint8_t *copy_to),
				 int (*write_block)(uint32_t lba, const uint8_t *copy_from));

int msc_control_request(usbd_device *usbd_dev,
				struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
				void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req));
void msc_data_tx_cb(usbd_device *usbd_dev, uint8_t ep);
void msc_data_rx_cb(usbd_device *usbd_dev, uint8_t ep);


#endif /* USB_MSC_LIB_H */

