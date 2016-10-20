#include<linux/moduleparam.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kobject.h>
#include<linux/sysfs.h>
#include"swap_string.h"
#include"sum_tree.h"
#define INPUTSIZE 100
static char *name1 = "swap_string";
static char *name2 = "calc";
static char *name3 = "sum_tree";
static unsigned int mask = 111;
static char swap_string_ctnr[INPUTSIZE];
static char calc_ctnr[INPUTSIZE];
static char sum_tree_ctnr[INPUTSIZE];
struct kobject *hw1;
static ssize_t user_store(struct kobject *kobj, struct kobj_attribute *attr, const char* buf, size_t count);
static ssize_t user_show(struct kobject *kobj, struct kobj_attribute *attr,  char* buf);

module_param(mask, uint, S_IRUGO);
module_param(name1, charp, S_IRUGO);
module_param(name2, charp, S_IRUGO);
module_param(name3, charp, S_IRUGO);

struct kobj_attribute hw1_swap_string = __ATTR(swap_string, 0660, user_show, user_store);
struct kobj_attribute hw1_calc        = __ATTR(calc, 0660, user_show, user_store);
struct kobj_attribute hw1_sum_tree    = __ATTR(sum_tree, 0660, user_show, user_store);

static int __init create(void)
{
	hw1_swap_string.attr.name = name1;
	hw1_calc.attr.name        = name2;
	hw1_sum_tree.attr.name    = name3;
	hw1 = kobject_create_and_add("hw1", kernel_kobj);

	if(mask/100)    sysfs_create_file(hw1, &hw1_swap_string.attr);
	if(mask%100/10) sysfs_create_file(hw1, &hw1_calc.attr);
	if(mask%10)     sysfs_create_file(hw1, &hw1_sum_tree.attr);

	return 0;
}
static void __exit cleanup(void) { kobject_del(hw1); }

static ssize_t user_store(struct kobject *kobj, struct kobj_attribute *attr, const char* buf, size_t count)
{
	if(!strcmp(attr->attr.name, name1))
		strcpy(swap_string_ctnr, swap_string_solve(buf));
/*
 if(!strcmp(attr->attr.name, name2))
	 strcpy(calc_ctnr, calc_solve(buf));
*/
	if(!strcmp(attr->attr.name, name3))
		strcpy(sum_tree_ctnr, sum_tree_solve(buf));

	return count;
}
static ssize_t user_show(struct kobject *kobj, struct kobj_attribute *attr,  char* buf)
{
	if(!strcmp(attr->attr.name, name1))
		return sprintf(buf, "%s", swap_string_ctnr);
	if(!strcmp(attr->attr.name, name2))
		return sprintf(buf, "%s", calc_ctnr);
	if(!strcmp(attr->attr.name, name3))
		return sprintf(buf, "%s", sum_tree_ctnr);

	return 0;
}

module_init(create);
module_exit(cleanup);

MODULE_LICENSE("Dual BSD/GPL");
