#include<linux/moduleparam.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kobject.h>
#include<linux/sysfs.h>
static char *name1 = "swap_string";
static char *name2 = "calc";
static char *name3 = "sum_tree";
static unsigned int mask = 111;
char* swap_string_ctnr;
char* calc_ctnr;
char* sum_tree_ctnr;
struct kobject *hw1;
static ssize_t user_store(struct kobject *kobj, struct kobj_attribute *attr, const char* buf, size_t count);
static ssize_t user_show(struct kobject *kobj, struct kobj_attribute *attr,  char* buf);
char* swap_string_solve(const char*);
char* calc_solve(const char*);
char* sum_tree_solve(const char*);

module_param(mask, uint, S_IRUGO);
module_param(name1, charp, S_IRUGO);
module_param(name2, charp, S_IRUGO);
module_param(name3, charp, S_IRUGO);

struct kobj_attribute hw1_swap_string = __ATTR(name1, 0660, user_show, user_store);
struct kobj_attribute hw1_calc        = __ATTR(name2, 0660, user_show, user_store);
struct kobj_attribute hw1_sum_tree    = __ATTR(name3, 0660, user_show, user_store);

static int __init homework_solution(void)
{
	hw1 = kobject_create_and_add("hw1", kernel_kobj);
	sysfs_create_file(hw1, &hw1_swap_string.attr);
	sysfs_create_file(hw1, &hw1_calc.attr);
	sysfs_create_file(hw1, &hw1_sum_tree.attr);
	printk(KERN_ALERT "****************\n");
	printk(KERN_ALERT "Hello\n");
	printk(KERN_ALERT "****************\n");

	return 0;
}

static void __exit cleanup(void)
{
	printk(KERN_ALERT "****************\n");
	printk(KERN_ALERT "Goodbye\n");
	printk(KERN_ALERT "****************\n");
}

static ssize_t user_store(struct kobject *kobj, struct kobj_attribute *attr, const char* buf, size_t count)
{
	const char* name = attr->attr.name;
	if(!strcmp(name, (const char*)name1))
		swap_string_ctnr = swap_string_solve(buf);
	if(!strcmp(name, (const char*)name2))
		calc_ctnr = calc_solve(buf);
	if(!strcmp(name, (const char*)name3))
		sum_tree_ctnr = sum_tree_solve(buf);

	return count;
}
static ssize_t user_show(struct kobject *kobj, struct kobj_attribute *attr,  char* buf)
{
	const char* name = attr->attr.name;
	if(!strcmp(name, (const char*)name1))
		return sprintf(buf, "%s\n", swap_string_ctnr);
	if(!strcmp(name, (const char*)name2))
		return sprintf(buf, "%s\n", calc_ctnr);
	if(!strcmp(name, (const char*)name3))
		return sprintf(buf, "%s\n", sum_tree_ctnr);

	return 0;
}

/*
char* swap_string_solve(const char* inp)
{
	return (char*)"pro1";
}
char* calc_solve(const char* inp)
{
	return (char*)"pro2";
}
char* sum_tree_solve(const char* inp)
{
	return (char*)"pro3";
}
*/

module_init(homework_solution);
module_exit(cleanup);

MODULE_LICENSE("Dual BSD/GPL");
