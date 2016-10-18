#include<linux/moduleparam.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kobject.h>
#include<linux/sysfs.h>
static char *name1 = (char*)"swap_string";
static char *name2 = (char*)"calc";
static char *name3 = (char*)"sum_tree";
static int mask = 111;
struct hw1_kobj
{
	char* swap_string_ctnr;
	char* calc_ctnr;
	char* sum_tree_ctnr;
	struct kobject kobj;
};
static struct hw1_kobj hw1;
static void hw1_release(struct kobject*);
static ssize_t user_store(struct kobject*, struct attribute*, char*);
static ssize_t user_show(struct kobject*, struct attribute*, const char*, size_t);
char* swap_string_solve(const char* inp);
char* calc_solve(const char* inp);
char* sum_tree_solve(const char* inp);

module_param(mask, int, 0666);
module_param(name1, charp, 0666);
module_param(name2, charp, 0666);
module_param(name3, charp, 0666);

static struct attribute swap_string_attr = {
	.name = name1,
	.owner = NULL,
	.mode = 0666
};
static struct attribute calc_attr = {
	.name = name2,
	.owner = NULL,
	.mode = 0666
};
static struct attribute sum_tree_attr = {
	.name = name3,
	.owner = NULL,
	.mode = 0666
};

static int __init homework_solution(void)
{
	struct attribute hw1_attrs[] = {&swap_string_attr, &calc_attr, &sum_tree_attr};
	struct sysfs_ops hw1_ops;
	struct kobj_type hw1_type;

	hw1_type->release = hw1_release;
	hw1_type->default_attrs = hw1_attrs;

	hw1_ops->store = user_store;
	hw1_ops->show = user_show;
	hw1_type->sysfs_ops = hw1_ops;

	memset(&hw1->kobj, 0, sizeof(struct kobject));
	if(kobject_init_and_add(&hw1->kobj, hw1_type, NULL, "NICONICONI")) kobject_put(&hw1->kobj);

	printk(KERN_ALERT "****************\n");
	printk(KERN_ALERT "oh yes~~~~~~~~~~\n");
	printk(KERN_ALERT "****************\n");

	return 0;
}

static void __exit cleanup(void)
{
	kobject_del(&hw1->kobj);

	printk(KERN_ALERT "****************\n");
	printk(KERN_ALERT "Goodbye\n");
	printk(KERN_ALERT "****************\n");
}

static void hw1_release(struct kobject *kobj)
{
	printk(KERN_ALERT "release kobject\n");
	kobject_del(kobj);
}

static ssize_t user_store(struct kobject *kobj, struct attribute *attr, const char *buf, size_t len)
{
	if(!strcmp(attr->name, (const char*)name1))
		hw1->swap_string_ctnr = swap_string_solve(buf);
	if(!strcmp(attr->name, (const char*)name2))
		hw1->calc_ctnr = calc_solve(buf);
	if(!strcmp(attr->name, (const char*)name3))
		hw1->sum_tree_ctnr = sum_tree_solve(buf);
}
static ssize_t user_show(struct kobject *kobj, struct attribute *attr,  char *buf)
{
	if(!strcmp(attr->name, (const char*)name1))
		return sprintf(buf, "%s\n", hw1->swap_string_ctnr);
	if(!strcmp(attr->name, (const char*)name2))
		return sprintf(buf, "%s\n", hw1->calc_ctnr);
	if(!strcmp(attr->name, (const char*)name3))
		return sprintf(buf, "%s\n", hw1->sum_tree_ctnr);
}

module_init(homework_solution);
module_exit(cleanup);

MODULE_LICENSE("Dual BSD/GPL");
