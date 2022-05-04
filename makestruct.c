#include "myfs.h"

void set_date(Date_pointer tmp) {
	time_t now = time(NULL);
	struct tm* t = localtime(&now);

	tmp->year = t->tm_year + 1900;
	tmp->month = t->tm_mon + 1;
	tmp->day = t->tm_mday;
	tmp->hour = t->tm_hour;
	tmp->minute = t->tm_min;
	tmp->second = t->tm_sec;
}

Super_block_pointer make_super_block(void) {
	Super_block_pointer temp = (Super_block_pointer)malloc(sizeof(Super_block));
	temp->D_bit_map = (Data_block_bit_map_pointer)malloc(sizeof(Data_block_bit_map));
	temp->D_bit_map->Super_block_data_block = (unsigned*)malloc(sizeof(unsigned) * 8);
	temp->I_bit_map = (Inode_bit_map_pointer)malloc(sizeof(Inode_bit_map));
	temp->I_bit_map->Super_block_inode = (unsigned*)malloc(sizeof(unsigned) * 4);

	return temp;
}

char* make_data_block(void) {
	char* temp = (char*)malloc(sizeof(sizeof(char) * MAX_BLOCK_SIZE));

	return temp;
}


Data_block_pointer make_data_blocks(void) {
	Data_block_pointer temp = (Data_block_pointer)malloc(sizeof(Data_block) * MAX_DATA_BLOCK_COUNT);

	for (int i = 0; i < MAX_DATA_BLOCK_COUNT; i++)
	{
		(temp + i)->data = make_data_block();

		(temp + i)->data_size = 0;
	}

	return temp;
}

Data_block_list make_data_block_list(void) {
	Data_block_list temp = (Data_block_list)malloc(sizeof(struct data_block_list));

	temp->data_blocks = make_data_blocks();

	temp->block_count = 0;

	return temp;
}

Inode_pointer make_inode_list(void) {
	Inode_pointer temp = (Inode_pointer)malloc(sizeof(Inode) * MAX_INODE_COUNT);

	for (int i = 0; i < MAX_INODE_COUNT; i++)
	{
		(temp + i)->node_type = 0;

		(temp + i)->create_time = (Date_pointer)malloc(sizeof(Date));

		(temp + i)->node_size = 0;

		(temp + i)->indir_block = 0;

		(temp + i)->node_name = (char*)malloc(sizeof(char) * 16);

		(temp + i)->D_block = (int*)calloc(8,sizeof(int));
	}

	return temp;
}

My_file_system_pointer make_file_system(void) {
	My_file_system_pointer temp = (My_file_system_pointer)malloc(sizeof(My_file_system));

	temp->S_block = make_super_block();

	temp->D_list = make_data_block_list();

	temp->I_list = make_inode_list();

	return temp;
}
