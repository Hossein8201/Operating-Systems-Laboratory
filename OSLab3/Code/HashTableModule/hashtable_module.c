#include <linux/module.h>    // برای ماژول‌های هسته
#include <linux/kernel.h>    // برای توابع هسته مانند printk
#include <linux/init.h>      // برای ماکروهای __init و __exit
#include <linux/slab.h>      // برای توابع مدیریت حافظه مانند kmalloc و kfree
#include <linux/hashtable.h> // برای توابع hashtable

// تعریف ساختار داده‌ای برای ذخیره در hashtable
struct my_data {
    int key;
    int value;
    struct hlist_node node;  // برای پیوند دادن به hashtable
};

// تعریف hashtable
#define HASHTABLE_SIZE 10  // اندازه hashtable
static DEFINE_HASHTABLE(my_hashtable, HASHTABLE_SIZE);

// تابعی که هنگام بارگذاری ماژول اجرا می‌شود
static int __init hashtable_module_init(void) {
    struct my_data *data;
    int i;

    printk(KERN_INFO "Starting hashtable module\n");

    // افزودن چند عنصر به hashtable
    for (i = 0; i < 5; i++) {
        data = kmalloc(sizeof(*data), GFP_KERNEL);
        if (!data) {
            printk(KERN_ERR "Failed to allocate memory\n");
            return -ENOMEM;
        }

        data->key = i;
        data->value = i * 100;

        // افزودن عنصر به hashtable
        hash_add(my_hashtable, &data->node, data->key);

        printk(KERN_INFO "Added: Key=%d, Value=%d\n", data->key, data->value);
    }

    // جستجو و چاپ یک عنصر
    int search_key = 3;
    struct my_data *found_data;
    hash_for_each_possible(my_hashtable, found_data, node, search_key) {
        if (found_data->key == search_key) {
            printk(KERN_INFO "Found: Key=%d, Value=%d\n", found_data->key, found_data->value);
            break;
        }
    }

    return 0;
}

// تابعی که هنگام حذف ماژول اجرا می‌شود
static void __exit hashtable_module_exit(void) {
    struct my_data *data;
    struct hlist_node *tmp;
    int bkt;

    printk(KERN_INFO "Exiting hashtable module\n");

    // پیمایش و حذف تمام عناصر hashtable
    hash_for_each_safe(my_hashtable, bkt, tmp, data, node) {
        printk(KERN_INFO "Removing: Key=%d, Value=%d\n", data->key, data->value);
        hash_del(&data->node);  // حذف عنصر از hashtable
        kfree(data);            // آزاد کردن حافظه
    }
}

// ثبت توابع init و exit
module_init(hashtable_module_init);
module_exit(hashtable_module_exit);

// اطلاعات ماژول
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hossein Tatar & Mohammad Amin Farahbakhsh");
MODULE_DESCRIPTION("A simple module to demonstrate hashtable in the kernel");
