#include <stdio.h>
#include <stdlib.h>

struct MEMORY{ 
	int page_num; 
	int reference_bit; 
	int dirty_bit; 
};

struct PAGE{
	int memory_location;
	int valid_bit;
};

int main(int argc, char *argv[]){
	int memory_size = atoi(argv[1]);
	int page_num, write;
	int memory_loc, outpage_num, search=0, diskhead_old=0, diskhead_new=0, diskhead=0;
	int pagefault_result=0, write_num=0, diskhead_result=0, clock=0;
	int i, empty=memory_size;

	FILE * input_file = fopen("a.txt", "r");
	struct MEMORY *memory = (struct MEMORY *) malloc(memory_size * sizeof(struct MEMORY));
	struct PAGE *page = (struct PAGE *) malloc(200000 * sizeof(struct PAGE));

	if(input_file==NULL){
		printf("No file exists.\n");
		return -1;
	}

	for(i=0;i < memory_size; i++){
		memory[i].reference_bit=0;
		memory[i].dirty_bit=0;
	}
	for(i=0; i<200000; i++) page[i].valid_bit=0;

	while(!feof(input_file)){
		clock=clock%memory_size;
		fscanf(input_file, "%d %d", &page_num, &write);

		if(page[page_num].valid_bit==1){
			memory_loc = page[page_num].memory_location;
			memory[memory_loc].reference_bit=1;
			if(write==1) memory[memory_loc].dirty_bit=1;
		}
		else{
			pagefault_result++;
			if(empty==0){
				while(memory[clock].reference_bit==1){
					memory[clock].reference_bit=0; 
					clock=(clock+1)%memory_size;
				}
				outpage_num = memory[clock].page_num;
				page[outpage_num].valid_bit=0;
				if(memory[clock].dirty_bit==1){
					write_num=write_num+1;
					diskhead_new = memory[clock].page_num/1000;
					if(diskhead_new > diskhead_old) diskhead= diskhead_new-diskhead_old;
					else diskhead= diskhead_old-diskhead_new;
					diskhead_result = diskhead_result+diskhead;
					diskhead_old=diskhead_new;
					diskhead_new=0;
					goto ADD;
				}
				else if(memory[clock].dirty_bit==0) goto ADD;
			}

			ADD:
			{diskhead_new = page_num/1000;
			if(diskhead_new > diskhead_old) diskhead= diskhead_new-diskhead_old;
			else diskhead= diskhead_old-diskhead_new;
			diskhead_result = diskhead_result+diskhead;
			diskhead_old=diskhead_new;
			diskhead_new=0;

			memory[clock].page_num=page_num;
			if(write==1) memory[clock].dirty_bit=1;
			else if(write==0) memory[clock].dirty_bit=0;
			memory[clock].reference_bit=0;
			page[page_num].memory_location=clock;
			page[page_num].valid_bit=1;
			clock++; 
			if(empty!=0) empty--; }
		}
	}

	printf("%d\n",pagefault_result);
	printf("%d\n", write_num);
	printf("%d\n", diskhead_result);

	fclose(input_file);
	return 0;
}