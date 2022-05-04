#include "myfs.h"

void mymkfs(My_file_system_pointer myfs) {
	
	free(myfs);
	myfs = make_file_system();

	set_date(myfs->I_list->create_time);
	*(myfs->I_list->D_block) = 1;
	myfs->I_list->node_type = 1;
	sprintf(myfs->I_list->node_name, "root");
	char* firstdata = (char*)malloc(sizeof(char) * 10);
	sprintf(firstdata, ". 1/.. 1");
	strcpy(myfs->D_list->data_blocks->data, firstdata);
	myfs->D_list->data_blocks->data_size = strlen(firstdata);
	myfs->D_list->block_count = 1;
	myfs->I_list->node_size = strlen(firstdata);
	*(myfs->S_block->D_bit_map->Super_block_data_block) = 1;
	*(myfs->S_block->I_bit_map->Super_block_inode) = 1;
	free(firstdata);
}

void myls(My_file_system_pointer myfs, char* ob1, char* path) {

	int inode = Search_inode(myfs, ob1, path);
	
	Inode_pointer temp;
	temp = myfs->I_list + inode - 1;

	if (strcmp(ob1, "\0") == 0) {

		int slash_cnt = char_count((myfs->D_list->data_blocks + *(temp->D_block) - 1)->data, '/');
		int num = slash_cnt + 1;

		char* string = (char*)malloc(sizeof(char) * 256);
		strcpy(string, (myfs->D_list->data_blocks + *(temp->D_block) - 1)->data);
		char* ptr = strtok(string, "/");

		char** names_p = (char**)malloc(sizeof(char*) * num);

		for (int i = 0;i < num;i++) {
			*(names_p + i) = (char*)malloc(sizeof(char) * 8);
			if (ptr != NULL) {
				sscanf(ptr, "%s", *(names_p + i));
				ptr = strtok(NULL, "/");
			}
		}

		qsort(names_p, num, sizeof(char*), comp_voca);

		int temp_inode;
		for (int i = 0;i < num;i++) {
			char* string1 = (char*)malloc(sizeof(char) * 16);
			strcat(strcat(string1, *(names_p + i)), " ");
			char* str_temp = (char*)malloc(sizeof(char) * 256);
			strcpy(str_temp, (myfs->D_list->data_blocks + *(temp->D_block) - 1)->data);
			char* str = strstr(str_temp, string1);

			sscanf(str, "%*s %d", &temp_inode);

			Inode_pointer tmp = (myfs->I_list + (temp_inode - 1));

			printf("%d/%d/%d %d:%d:%d ", tmp->create_time->year, tmp->create_time->month, tmp->create_time->day, tmp->create_time->hour, tmp->create_time->minute, tmp->create_time->second);
			switch (tmp->node_type) {

			case 1:
				printf("directory ");
				break;
			case 2:
				printf("file ");
				break;
			}

			printf("%d %d byte %s\n", temp_inode, tmp->node_size, *(names_p + i));

			free(string1);
			free(str_temp);
		}
		free(string);
		for (int i = 0;i < num;i++)
			free(*(names_p + i));
		free(names_p);
		
	}
	else {
		temp = (myfs->I_list + inode - 1);

		printf("%d/%d/%d %d:%d:%d ", temp->create_time->year, temp->create_time->month, temp->create_time->day, temp->create_time->hour, temp->create_time->minute, temp->create_time->second);
		switch (temp->node_type) {

		case 1:
			printf("directory ");
			break;
		case 2:
			printf("file ");
			break;
		}

		printf("%d %d byte\n", inode, temp->node_size);
	}



}

void mycat(My_file_system_pointer myfs, char* ob1, char* path) {

	int inode = Search_inode(myfs, ob1, path);

	int j = 0;
	while (*((myfs->I_list + inode - 1)->D_block + j)) {
		printf("%s", (myfs->D_list->data_blocks + *((myfs->I_list + inode - 1)->D_block + j))->data);
		j++;
	}
	printf("\n");

}

void myshowfile(My_file_system_pointer myfs, char* ob1, char* ob2, char* ob3, char* path) {

	int inode = Search_inode(myfs, ob3, path);
	for (int i = atoi(ob1);i <= atoi(ob2);i++) {
		printf("%c", *((myfs->D_list->data_blocks + inode - 1)->data + i - 1));
	}
	printf("\n");
}

void mypwd(char* path) {
	printf("%s\n", path);
}

void mycpto(My_file_system_pointer myfs, char* ob1, char* ob2, char* path) {

	int inode = Search_inode(myfs, ob1, path);
	
	FILE* opt = fopen(ob2, "w");
	
	int i = 0;
	while (*((myfs->I_list + inode -1)->D_block + i)) {
		fprintf(opt, "%s", (myfs->D_list->data_blocks + *((myfs->I_list + inode-1)->D_block + i))->data);
		i++;
	}
	
	fclose(opt);
	
}

void mycpfrom(My_file_system_pointer myfs, char* ob1, char* ob2, char* path) {

	int inode = Search_inode(myfs, "\0", path), data_cnt = 0, D_block_cnt = 0, file_size = 0, inode_cnt = 0, data_block_cnt = 0;

	while ((myfs->I_list + inode_cnt)->node_type)
		inode_cnt++;
	while ((myfs->D_list->data_blocks + data_block_cnt)->data_size)
		data_block_cnt++;
	(myfs->I_list + inode_cnt)->node_type = 2;
	*((myfs->I_list + inode_cnt)->D_block + D_block_cnt) = data_block_cnt;
	set_date((myfs->I_list + inode_cnt)->create_time);
	
	unsigned inode_mask = 1, data_block_mask = 1;
	inode_mask <<= inode_cnt%32;
	*(myfs->S_block->I_bit_map->Super_block_inode + (inode_cnt/32)) |= inode_mask;

	FILE* ipt=fopen(ob1,"r");
	if (ipt == NULL)
		return;

	char* c = (char*)malloc(sizeof(char) * 2);
	char* temp = (char*)calloc(256,sizeof(char));
	
	while (feof(ipt)==0) {
		fscanf(ipt, "%1[^\n]s", c);
		
		data_cnt++;
		file_size++;
		strcat(temp, c);
		
		if (data_cnt > 255) {
			strcpy((myfs->D_list->data_blocks + data_block_cnt)->data, temp);
			(myfs->D_list->data_blocks + data_block_cnt)->data_size = data_cnt;
			data_block_mask <<= data_block_cnt % 32;
			*(myfs->S_block->D_bit_map->Super_block_data_block + (data_block_cnt / 32)) |= data_block_mask;
			data_block_mask = 1;
			myfs->D_list->block_count++;
			free(temp);

			temp = (char*)malloc(sizeof(char) * 256);
			
			D_block_cnt++;
			data_block_cnt++;
			while ((myfs->D_list->data_blocks + data_block_cnt)->data_size)
				data_block_cnt++;
			*((myfs->I_list + inode_cnt)->D_block + D_block_cnt) = data_block_cnt;
			data_cnt = 0;
			
		}
		
	}
	strcpy((myfs->D_list->data_blocks + data_block_cnt)->data, temp);
	(myfs->D_list->data_blocks + data_block_cnt)->data_size = data_cnt;
	data_block_mask <<= data_block_cnt % 32;
	*(myfs->S_block->D_bit_map->Super_block_data_block + (data_block_cnt / 32)) |= data_block_mask;
	myfs->D_list->block_count++;
	free(temp);

	strcpy((myfs->I_list + inode_cnt)->node_name, ob2);
	(myfs->I_list + inode_cnt)->node_size = file_size;
	
	char* string = (char*)malloc(sizeof(char) * 10);
	sprintf(string, "/%s %d", ob2, inode_cnt + 1);
	strcat((myfs->D_list->data_blocks + (*(myfs->I_list + inode-1)->D_block)-1)->data, string);
	(myfs->D_list->data_blocks + (*(myfs->I_list + inode-1)->D_block))->data_size += strlen(string);

	free(c);
	free(string);
	fclose(ipt);
	
}


void myinode(My_file_system_pointer myfs, char* ob1) {
	int arg = atoi(ob1);
	Inode_pointer temp = (myfs->I_list + arg - 1);
	printf("type : ");
	switch (temp->node_type) {
	case 0:
		printf("Not in use.\n");
		return;
	case 1:
		printf("directory\n");
		break;
	case 2:
		printf("file\n");
		break;
	}
	
	printf("create date : %d/%d/%d %d:%d:%d\n", temp->create_time->year, temp->create_time->month, temp->create_time->day, temp->create_time->hour, temp->create_time->minute, temp->create_time->second);
	printf("size : %d\n", temp->node_size);
	printf("direct block list :\n");
	for (int i = 0;i < 8;i++) {
		if (temp->D_block + i)
			printf("#%d direct data block : %d\n", i, *(temp->D_block + i));
	}
	printf("indirect block list : %d\n", temp->indir_block);
}

void mydatablock(My_file_system_pointer myfs, char* ob1) {
	int arg = atoi(ob1);
	printf("%s", (myfs->D_list->data_blocks + arg - 1)->data);
	printf("\n");
}

void mystate(My_file_system_pointer myfs) {

	int Inode_count = 0, Data_size = 0;

	for (int i = 0; i < MAX_INODE_COUNT; i++)
	{
		if ((myfs->I_list + i)->node_type) {
			Inode_count++;
		}
			
	}

	for (int i = 0; i < MAX_DATA_BLOCK_COUNT; i++)
	{
		Data_size += (myfs->D_list->data_blocks + i)->data_size;
		
	}

	printf("Inode state : \n");
	printf("    Total : 128\n");
	printf("    Used : %d\n", Inode_count);
	printf("    Available : %d\n", 128 - Inode_count);
	printf("    Inode Map :");
	for (int i = 0;i < 4;i++) {
		if (i % 2 == 0)
			printf("\n     ");
		bit_print(*(myfs->S_block->I_bit_map->Super_block_inode+i));
	}

	printf("\nData Block state : \n");
	printf("    Total : 256 blocks / 65536 byte\n");
	printf("    Used : %d blocks / %d byte\n", myfs->D_list->block_count, Data_size);
	printf("    Available : %d blocks / %d byte\n", 256 - myfs->D_list->block_count, 65536 - Data_size);
	printf("    Data Block Map :");
	for (int i = 0;i < 8;i++) {
		if (i % 2 == 0)
			printf("\n     ");
		bit_print(*(myfs->S_block->D_bit_map->Super_block_data_block+i));
	}
	printf("\n");
}
