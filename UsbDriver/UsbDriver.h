#ifndef USBDRIVER_H
#define USBDRIVER_H
//Dipak Malla
//mmallad
//USB Driver Example
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define AUTHOR "Dipak Malla"
#define DESCRIPTION "USB Device Driver Example"
#define LICENSE "GPL"

#define BULK_EP_OUT 0x01
#define BULK_EP_IN 0x82
#define MAX_PKT_SIZE 512

//Define Funtion Prototypes
static int __init start(void);
static void __exit end(void);
static int usb_prob(struct usb_interface*,const struct usb_device_id*);
static void usb_desconnect(struct usb_interface*);
static int usb_open(struct inode*, struct file*);
static int usb_close(struct inode*,struct file*);
static ssize_t usb_write(struct file*, const char*,size_t, loff_t*);
static ssize_t usb_read(struct file*, char*,size_t,loff_t*);
#endif // USBDRIVER_H
