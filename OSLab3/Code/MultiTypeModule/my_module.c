#include <linux/module.h>       // برای ماژول‌های هسته
#include <linux/kernel.h>       // برای توابع هسته مانند printk
#include <linux/init.h>         // برای ماکروهای __init و __exit
#include <linux/moduleparam.h>  // برای ماکروهای module_param و MODULE_PARM_DESC

// تعریف پارامترها
static short my_short = 0;
static int my_int = 0;
static long my_long = 0;
static char *my_string = "default";
static int my_array[5] = {0, 0, 0, 0, 0};
static int arr_argc = 0;

// تعریف پارامترها برای ماژول
module_param(my_short, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(my_short, "A short integer");

module_param(my_int, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(my_int, "An integer");

module_param(my_long, long, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(my_long, "A long integer");

module_param(my_string, charp, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(my_string, "A string");

module_param_array(my_array, int, &arr_argc, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(my_array, "An array of integers");

// تابعی که هنگام بارگذاری ماژول اجرا می‌شود
static int __init my_module_init(void) {
    int i;
    printk(KERN_INFO "Hello, Kernel World!\n");
    printk(KERN_INFO "my_short: %hd\n", my_short);
    printk(KERN_INFO "my_int: %d\n", my_int);
    printk(KERN_INFO "my_long: %ld\n", my_long);
    printk(KERN_INFO "my_string: %s\n", my_string);
    for (i = 0; i < arr_argc; i++) {
        printk(KERN_INFO "my_array[%d]: %d\n", i, my_array[i]);
    }
    return 0;
}

// تابعی که هنگام حذف ماژول اجرا می‌شود
static void __exit my_module_exit(void) {
    printk(KERN_INFO "Goodbye, Kernel World!\n");
}

// ثبت توابع init و exit
module_init(my_module_init);
module_exit(my_module_exit);

// اطلاعات ماژول
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hossein Tatar & Amin FarahBakhsh");
MODULE_DESCRIPTION("A simple module to demonstrate module parameters");
