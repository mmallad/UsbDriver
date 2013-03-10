#include "UsbDriver.h"


static int return_value = -1;
static struct usb_device *dev;
static struct usb_class_driver u_cls;
static unsigned char bulk_buf[MAX_PKT_SIZE];
static int readCount = 0;

//The struct type usb_device_id which maps with device
static struct usb_device_id usb_table[] =
{
    { USB_DEVICE(0x0951,0x1642)},
    {}
};
static struct usb_driver my_usb = {

    .name = "This is my usb :)",
    .id_table = usb_table,
    .probe = usb_prob,
    .disconnect = usb_desconnect
};
static struct file_operations fops = {
    .read = usb_read,
    .write = usb_write,
    .open = usb_open,
    .release = usb_close
};
//This is prob function. Which will be called when the Device is connected to the bus
static int usb_prob(struct usb_interface *intf, const struct usb_device_id *id)
{
    dev = interface_to_usbdev(intf);
    u_cls.name = "usb/pen%d";
    u_cls.fops = &fops;
    printk(KERN_INFO "My USB Plugged In !\n Vendor ID: %d, Product ID: %d",id->idVendor,id->idProduct);
    if((return_value = usb_register_dev(intf,&u_cls)) < 0)
    {
        printk(KERN_ALERT "Oops something went wrong while registering the device.\n");
    }
    else
    {
        printk(KERN_INFO "Thank God we got minor number %d",intf->minor);
    }
    return return_value;
}
//The function called when Device is removed from bus
static void usb_desconnect(struct usb_interface *intf)
{
    usb_deregister_dev(intf, &u_cls);
    printk(KERN_INFO "Device successfully disconnected :)\n");
}
MODULE_DEVICE_TABLE(usb,usb_table);

//Struct of type usb_driver which tell the kernel about device name we gave. id table which maps to the device.
//prob function when device is plugged ib and
//disconnect function which is called when device is unplugged .

static int usb_open(struct inode *ind, struct file *f)
{
    return 0;
}
static int usb_close(struct inode *ind, struct file *f)
{
    return 0;
}
static ssize_t usb_read(struct file *f, char *buffer, size_t buffer_len, loff_t *offsetCounter)
{
    readCount = 0;
    return_value = usb_bulk_msg(dev,usb_rcvbulkpipe(dev,BULK_EP_IN),bulk_buf,MAX_PKT_SIZE,&readCount,5000);
    if(return_value < 0)
    {
        printk(KERN_ALERT "Could not read bulk message.\n");
        return return_value;
    }
    if(copy_to_user(buffer,bulk_buf,buffer_len))
    {
        return -EFAULT;
    }
    printk(KERN_INFO "I am reading from usb drive.");
    return 0;
}
static ssize_t usb_write(struct file *f, const char *buffer, size_t buffer_len, loff_t *offsetCounter)
{
    //Actaully this is write Count but I am very lazy solti.
    readCount = 0;
    if(copy_from_user(bulk_buf,buffer,buffer_len))
    {
        return -EFAULT;
    }
    return_value = usb_bulk_msg(dev,usb_sndbulkpipe(dev,BULK_EP_OUT),bulk_buf,MAX_PKT_SIZE,&readCount,5000);
    if(return_value < 0)
    {
        printk(KERN_ALERT "Could not write to usb.\n");
        return return_value;
    }
    printk(KERN_INFO "I am writing to USB. \n");
    return 0;
}

static int __init start(void)
{
    printk(KERN_INFO "Hello I want to show my usb driver example :) \n");
    printk(KERN_INFO "USB Initialized \n");
    return_value = usb_register(&my_usb);
    if(return_value < 0)
    {
        printk(KERN_ALERT "Oops USB could not be registered. \n");
        return return_value;
    }
    printk(KERN_INFO "USB Registration was successfull. :) \n");
    return return_value;
}
static void __exit end(void)
{
    printk(KERN_INFO "We are going to un-register the USB. \n");
    usb_deregister(&my_usb);
    printk(KERN_INFO "Unregistration was complete :) \n");
}

MODULE_LICENSE(LICENSE);
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_SUPPORTED_DEVICE("My USB Only");

module_init(start);
module_exit(end);
