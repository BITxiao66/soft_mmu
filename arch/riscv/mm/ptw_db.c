/*
* Description: soft page-table-walk file
* Author: xiaoziyuan19s@ict.ac.cn
* Last modify:  16:20  Jan 26th,2021
* Version: v0.1
*/

#include <asm/current.h> //current
#include <asm/pgtable.h> //pte
#include <asm/pgtable-64.h> //PMD
#include <linux/mm_types.h> //mm_struct page
#include <linux/kernel.h> //printk
#include <linux/page_ref.h> //page_ref_inc
#include <linux/sched.h> //task_struct

static inline int pte_read(pte_t pte)
{
	return pte_val(pte) & _PAGE_READ;
}

static inline int pte_user(pte_t pte)
{
	return pte_val(pte) & _PAGE_USER;
}

static int pte_fine(pte_t pte,int level)
{
    int ret = 0;
    if (!pte_present(pte))
    {
        printk(KERN_WARNING  "Can not touch PTE!\n");
        goto out;
    } 
    if (!pte_read(pte) && pte_write(pte))
    {
        printk(KERN_WARNING "Illegal PTE format!\n");
        goto out;
    }
    if (!pte_read(pte) && !pte_write(pte) && !pte_exec(pte))
    {
        ret = level;
        goto out;
    }
    // must be leaf
    unsigned long huge_mask = ~((1UL<<(PAGE_SHIFT + (PMD_SHIFT-PAGE_SHIFT)*level))-1UL);
    if (level && (huge_mask & pte_val(pte)))
    {
        printk(KERN_WARNING "Got an wrong hugepage!\n");
        goto out;
    }
    if (!pte_user(pte))
    {
        printk(KERN_WARNING "It's not a user page!\n");
        goto out;
    }
    printk(KERN_INFO "Translate finish!\n");
    ret = 1;
out:
    printk(KERN_INFO "ret:%d!\n",ret);
    return ret;
}

static long ptw(struct mm_struct *mm, long offset)
{
    long ret = -1;
    unsigned long table = __virt_to_phys(mm->pgd);
    unsigned long *pt = (unsigned long*)mm->pgd;
    pte_t pte;
    long index;
    int i;
    for (i = 2; i >= 0; i--)
    {
        if (pt==NULL) 
        {
            printk(KERN_WARNING "An empty page!\n");
            goto out;
        }

        page_ref_inc(phys_to_page(table));
        if ( !pfn_valid(phys_to_pfn(table)))
        {
            printk(KERN_WARNING "Page num out of range!\n");
            goto out;
        }
        index = offset >> (PAGE_SHIFT + (PMD_SHIFT-PAGE_SHIFT)*i);
        index &= (1<<(PMD_SHIFT-PAGE_SHIFT))-1;
        pte.pte = pt[index];
        if ( !pte_fine(pte,i))
        {
            printk(KERN_WARNING "Address translate failed!\n");
            goto out;
        }
        pte.pte >>= 10;
        pte.pte <<= PAGE_SHIFT;
        page_ref_dec(phys_to_page(table));
        table = pte_val(pte);
        pt = __va(table);
    }
out:
    return ret;
}

// syscall entrance
long sys_riscv_ptwdb(long offset, long b)
{
    return ptw(current->mm, offset);
}
