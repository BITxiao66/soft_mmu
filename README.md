## soft_mmu
### Adds a software page-table-walker process to RISC-V's Linux 5.10.8 kernel.
---
* This project developed a syscall in the Linux 5.10.8 kernel, the call-number is 250, this syscall can let the user mode program probe the paging information, 
* it should be noted that under RISC-V architecture, TLB processing is automatically completed by the hardware, this process is transparent to the kernel.
* The project was expanded under Linux5.10.8/arch/riscv, and the files mm/ptw_db.c; include/asm/vdso.h; was mainly modified, replace the project in linux/arch/riscv than you can complete compile. 
* The copyright of the original code belongs to the original author.
---
本项目在Linux 5.10.8内核中开发了一个系统调用，调用号为250，该系统调用可以让用户态程序探测分页信息，需要注意的是，在RISC-V架构下，TLB处理由硬件自动完成，这个过程对内核是透明的。

项目在原Linux5.10.8/arch/riscv下拓展，主要修改了文件mm/ptw_db.c; include/asm/vdso.h; include/uapi/unistd.h，将项目替换在linux/arch/riscv下即可完成编译。原代码的著作权属于原作者。
