#include <linux/module.h>         // برای ماژول‌های هسته
#include <linux/kernel.h>         // برای توابع هسته مانند printk
#include <linux/init.h>           // برای ماکروهای __init و __exit
#include <linux/sched.h>          // برای دسترسی به ساختار task_struct
#include <linux/sched/signal.h>   // برای ماکرو for_each_process
#include <linux/mm.h>             // برای دسترسی به توابع مرتبط با حافظه

// تابعی که هنگام بارگذاری ماژول اجرا می‌شود
static int __init process_info_init(void) {
    struct task_struct *task;

    printk(KERN_INFO "Starting process info module\n");

    // پیمایش تمام فرایندهای سیستم
    for_each_process(task) {
        // چاپ اطلاعات فرایند
        printk(KERN_INFO "Process: %s (PID: %d)\n", task->comm, task->pid);
        printk(KERN_INFO "CPU Usage: %llu\n", (unsigned long long)task->utime + task->stime);

        // محاسبه میزان مصرف حافظه (RSS)
        if (task->mm) {
            unsigned long rss = get_mm_rss(task->mm) << PAGE_SHIFT;
            printk(KERN_INFO "Memory Usage: %lu bytes\n", rss);
        } else {
            printk(KERN_INFO "Memory Usage: 0 bytes (no mm_struct)\n");
        }

        printk(KERN_INFO "----------------------------------------\n");
    }

    return 0;
}

// تابعی که هنگام حذف ماژول اجرا می‌شود
static void __exit process_info_exit(void) {
    printk(KERN_INFO "Exiting process info module\n");
}

// ثبت توابع init و exit
module_init(process_info_init);
module_exit(process_info_exit);

// اطلاعات ماژول
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hossein Tatar & Mohammad Amin Farahbakhsh");
MODULE_DESCRIPTION("A module to print process information");
