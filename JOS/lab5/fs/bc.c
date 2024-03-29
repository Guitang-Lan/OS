
#include "fs.h"

// Return the virtual address of this disk block.
void*
diskaddr(uint32_t blockno)
{
	if (blockno == 0 || (super && blockno >= super->s_nblocks))
		panic("bad block number %08x in diskaddr", blockno);
	return (char*) (DISKMAP + blockno * BLKSIZE);
}

// Fault any disk block that is read in to memory by
// loading it from disk.
static void
bc_pgfault(struct UTrapframe *utf)
{
	void *addr = (void *) utf->utf_fault_va;
	uint32_t blockno = ((uint32_t)addr - DISKMAP) / BLKSIZE;
	int r;

	// Check that the fault was within the block cache region
	if (addr < (void*)DISKMAP || addr >= (void*)(DISKMAP + DISKSIZE)) {
		panic("page fault in FS: eip %08x, va %08x, err %04x",
		      utf->utf_eip, addr, utf->utf_err);
	}

	// Sanity check the block number.
	if (super && blockno >= super->s_nblocks)
		panic("reading non-existent block %08x\n", blockno);

	// Allocate a page in the disk map region, read the contents
	// of the block from the disk into that page.
	// Hint: first round addr to page boundary.
	//
	// LAB 5: you code here:
	// allocate a page in the fault addr
	void *round_addr = ROUNDDOWN(addr, PGSIZE);
	int err_code;
	if ((err_code = sys_page_alloc(0, round_addr, PTE_P | PTE_U | PTE_W)) < 0) {
		panic("bc_pgfault:sys_page_alloc:%e", err_code);
	}
	// load page from disk to the new allocated memory page
	uint32_t secno = ((uint32_t)round_addr - DISKMAP) / SECTSIZE;
	if ((err_code = ide_read(secno, round_addr, BLKSECTS)) < 0) {
		panic("bc_pgfault:ide_read");
	}
}


void
bc_init(void)
{
	struct Super super;
	set_pgfault_handler(bc_pgfault);

	// cache the super block by reading it once
	memmove(&super, diskaddr(1), sizeof super);
}

