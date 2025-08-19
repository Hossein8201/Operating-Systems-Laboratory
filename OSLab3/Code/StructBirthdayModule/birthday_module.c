#include <linux/module.h>   // برای ماژول‌های هسته
#include <linux/kernel.h>   // برای توابع هسته مانند printk
#include <linux/init.h>     // برای ماکروهای __init و __exit
#include <linux/slab.h>     // برای توابع مدیریت حافظه مانند kmalloc و kfree
#include <linux/list.h>     // برای لیست‌های پیوندی

// تعریف ساختار birthday
struct birthday {
    int day;
    int month;
    int year;
    struct list_head list;  // لیست پیوندی
};

// تعریف و مقداردهی اولیه لیست
static LIST_HEAD(birthday_list);

// تابعی که هنگام بارگذاری ماژول اجرا می‌شود
static int __init birthday_module_init(void) {
    int i;
    struct birthday *person;

    printk(KERN_INFO "Starting birthday module\n");

    // ایجاد پنج عنصر از نوع struct birthday و افزودن آن‌ها به لیست
    for (i = 0; i < 5; i++) {
        person = kmalloc(sizeof(*person), GFP_KERNEL);
        if (!person) {
            printk(KERN_ERR "Failed to allocate memory\n");
            return -ENOMEM;
        }

        // مقداردهی به عنصر
        person->day = i + 1;
        person->month = i + 1;
        person->year = 2000 + i;

        // افزودن عنصر به انتهای لیست
        list_add_tail(&person->list, &birthday_list);

        // چاپ اطلاعات عنصر
        printk(KERN_INFO "Added: Day=%d, Month=%d, Year=%d\n", person->day, person->month, person->year);
    }

    // پیمایش لیست و چاپ اطلاعات
    printk(KERN_INFO "Traversing the list:\n");
    list_for_each_entry(person, &birthday_list, list) {
        printk(KERN_INFO "Day=%d, Month=%d, Year=%d\n", person->day, person->month, person->year);
    }

    return 0;
}

// تابعی که هنگام حذف ماژول اجرا می‌شود
static void __exit birthday_module_exit(void) {
    struct birthday *person, *tmp;

    printk(KERN_INFO "Exiting birthday module\n");

    // پیمایش برعکس لیست و حذف هر عنصر
    printk(KERN_INFO "Traversing the list in reverse and freeing memory:\n");
    list_for_each_entry_safe_reverse(person, tmp, &birthday_list, list) {
        printk(KERN_INFO "Removing: Day=%d, Month=%d, Year=%d\n", person->day, person->month, person->year);
        list_del(&person->list);  // حذف عنصر از لیست
        kfree(person);            // آزاد کردن حافظه
    }
}

// ثبت توابع init و exit
module_init(birthday_module_init);
module_exit(birthday_module_exit);

// اطلاعات ماژول
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hossein Tatar Mohammad Amin Farahbakhsh");
MODULE_DESCRIPTION("A module to demonstrate linked lists in the kernel");
