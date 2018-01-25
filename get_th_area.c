
#include <errno.h>
#include <sys/stat.h>
#include <asm/ldt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define __NR_get_thread_area 244

void InitSegmentDescriptors() {

	struct user_desc* table_entry_ptr = NULL;

	/* Allocates memory for a user_desc struct */
	table_entry_ptr = (struct user_desc*)malloc(sizeof(struct user_desc));

	/* Fills the user_desc struct which represents the segment for mx */

	for (int i = 0; i < 0x100; ++i) {
		/* Writes a user_desc struct to the ldt */
		table_entry_ptr->entry_number = i;
		int ret = syscall( __NR_get_thread_area,
				table_entry_ptr);
		if (ret < 0 && errno == EINVAL) {
			//printf("[Child] Cannot access gdt entry %d. Errno %d\n", i, errno);
		} else {
			printf("[Child] GDT entry %d base address %08x limit %08x\n", i, table_entry_ptr->base_addr, table_entry_ptr->limit);
		}

	}
}

int main() {
	InitSegmentDescriptors();
	return 0;
}
