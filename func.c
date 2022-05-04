#include "myfs.h"

void Call_my_file_system(My_file_system_pointer myfs) {

	FILE* ipt;
	if ((ipt = fopen("myfs.txt", "r")) == NULL) {
		printf("파일시스템이 없습니다. 파일시스템을 만듭니다.\n");
		mymkfs(myfs);
		return;
	}

	for (int i = 0; i < 4; i++)
		fscanf(ipt, "%u\n", (myfs->S_block->I_bit_map->Super_block_inode+i));
	for (int i = 0;i < 8; i++)
		fscanf(ipt, "%u\n", (myfs->S_block->D_bit_map->Super_block_data_block+i));
	fscanf(ipt, "%d\n", &(myfs->D_list->block_count));
	for (int i = 0;i < MAX_DATA_BLOCK_COUNT;i++) {
		fscanf(ipt, "%d ", &((myfs->D_list->data_blocks + i)->data_size));
		if ((myfs->D_list->data_blocks + i)->data_size) {
			fscanf(ipt, "%[^\n]s\n", (myfs->D_list->data_blocks + i)->data);
		}
	}
		
	for (int i = 0;i < MAX_INODE_COUNT;i++) {
		fscanf(ipt, "%u %d %d %d %d %d %d %d %d ", &((myfs->I_list + i)->node_type), &((myfs->I_list + i)->create_time->year), &((myfs->I_list + i)->create_time->month), &((myfs->I_list + i)->create_time->day), &((myfs->I_list + i)->create_time->hour), &((myfs->I_list + i)->create_time->minute), &((myfs->I_list + i)->create_time->second), &((myfs->I_list + i)->node_size), &((myfs->I_list + i)->indir_block));
		for (int j = 0;j < 8;j++) {
			fscanf(ipt, "%d ", (myfs->I_list + i)->D_block + j);
		}
		if ((myfs->I_list + i)->node_type)
			fscanf(ipt, "%s\n", (myfs->I_list + i)->node_name);
		else
			fscanf(ipt, "\n");
	}

	fclose(ipt);
}

void Save_my_file_system(My_file_system_pointer myfs) {

	FILE* opt;
	opt = fopen("myfs.txt", "w");

	for (int i = 0; i < 4; i++)
		fprintf(opt, "%u\n", *(myfs->S_block->I_bit_map->Super_block_inode+i));
	for (int i = 0;i < 8; i++)
		fprintf(opt, "%u\n", *(myfs->S_block->D_bit_map->Super_block_data_block+i));
	fprintf(opt, "%d\n", myfs->D_list->block_count);
	for (int i = 0;i < MAX_DATA_BLOCK_COUNT;i++) {
		if ((myfs->D_list->data_blocks + i)->data_size > 0) {
			fprintf(opt, "%d %s\n", (myfs->D_list->data_blocks + i)->data_size, (myfs->D_list->data_blocks + i)->data);
		}
		else {
			fprintf(opt, "%d\n", (myfs->D_list->data_blocks + i)->data_size);
		}
	}
		
	for (int i = 0;i < MAX_INODE_COUNT;i++) {
		fprintf(opt, "%u %d %d %d %d %d %d %d %d ", (myfs->I_list + i)->node_type, (myfs->I_list + i)->create_time->year, (myfs->I_list + i)->create_time->month, (myfs->I_list + i)->create_time->day, (myfs->I_list + i)->create_time->hour, (myfs->I_list + i)->create_time->minute, (myfs->I_list + i)->create_time->second, (myfs->I_list + i)->node_size, (myfs->I_list + i)->indir_block);
		for (int j = 0;j < 8;j++) {
			fprintf(opt, "%d ", *((myfs->I_list + i)->D_block + j));
		}
		fprintf(opt, "%s", (myfs->I_list + i)->node_name);
		fprintf(opt, "\n");
	}
	fclose(opt);
}

int char_count(char* str, char c) {
	int cnt = 0;
	int i = 0;
	while (strcmp(str + i, "\0")) {
		if (*(str + i) == c)
			cnt++;
		i++;
	}
	return cnt;
}

int Search_inode(My_file_system_pointer myfs, char* ob1, char* path) {
	
	int ob1_bool = 1;

	if (strcmp(ob1, "\0")) {
		ob1_bool = 0;
	}
	
	int slash_cnt = char_count(path, '/');
	int inode = 1;
	
	if (slash_cnt == 1 && strlen(path) == 1) {
		char* string1 = (char*)malloc(sizeof(char) * 10);
		strcpy(string1, ob1);
		strcat(string1, " ");
		char* string = strstr((myfs->D_list->data_blocks + inode - 1)->data, string1);
		sscanf(string, "%*s %d", &inode);
	}
	
	else {

		char* temp = (char*)malloc(sizeof(path));
		strcpy(temp, path + 1);
		char* ptr = strtok(temp, "/");

		char** args;
		args = (char**)malloc(sizeof(char*) * (slash_cnt + ob1_bool));


		for (int i = 0;i <= slash_cnt;i++) {
			*(args + i) = (char*)malloc(sizeof(char) * 8);
			if (ptr != NULL) {
				strcpy(*(args + i), ptr);
				ptr = strtok(NULL, "/");
			}

			else
				strcpy(*(args + i), ob1);

		}

		for (int i = 0;i <= slash_cnt;i++) {
			char* string1 = (char*)malloc(sizeof(char) * 16);
			sprintf(string1, " ");
			strcat(strcat(string1, *(args+i)), " ");
			char* string = strstr((myfs->D_list->data_blocks + inode - 1)->data, string1);
			sscanf(string, "%*s %d", &inode);
		}
	}
	
	return inode;
	

}

void bit_print(unsigned a) {
	int n = sizeof(unsigned) * 8;
	unsigned mask = 1;
	for (int i = 1; i <= n; i++) {
		putchar(((a & mask) == 0) ? '0' : '1');
		mask <<= 1;
		if (i % 4 == 0 && i < n)
			putchar(' ');
	}
	putchar(' ');
}

int comp_voca(const void* p, const void* q) {
	return strcmp(*(char**)p, *(char**)q);
}


int commandCompare(My_file_system_pointer myfs, char* exam, char* ob1, char* ob2, char* ob3, char* path) {
	if (strcmp(exam, "mymkfs") == 0) {

		if (strcmp(ob1, "\0")) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			printf("파일시스템이 있습니다. 다시 만들겠습니까? (y/n)");
			char c;
			c = getchar();
			getchar();
			if (c == 'y' || c == 'Y')
				mymkfs(myfs);
			else if (c == 'n' || c == 'N');
		}

	}
	else if (strcmp(exam, "myls") == 0) {

		if (strcmp(ob2, "\0")) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			myls(myfs, ob1, path);
		}

	}
	else if (strcmp(exam, "mycat") == 0) {

		if (strcmp(ob2, "\0") || strcmp(ob1, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			mycat(myfs, ob1, path);
		}

	}
	else if (strcmp(exam, "myshowfile") == 0) {

		if (strcmp(ob3, "\0")) {
			myshowfile(myfs, ob1, ob2, ob3, path);
		}
		else {
			printf("잘못된 입력입니다.\n");
		}

	}
	else if (strcmp(exam, "mypwd") == 0) {

		if (strcmp(ob1, "\0")) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			mypwd(path);
		}

	}
	else if (strcmp(exam, "mycd") == 0) {

		if (strcmp(ob2, "\0")) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			//mycd();
		}

	}
	else if (strcmp(exam, "mycp") == 0) {

		if (strcmp(ob3, "\0") || strcmp(ob2, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			//mycp();
		}

	}
	else if (strcmp(exam, "mycpto") == 0) {

		if (strcmp(ob3, "\0") || strcmp(ob2, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			mycpto(myfs, ob1, ob2, path);
		}

	}
	else if (strcmp(exam, "mycpfrom") == 0) {

		if (strcmp(ob3, "\0") || strcmp(ob2, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			mycpfrom(myfs, ob1, ob2, path);
		}

	}
	else if (strcmp(exam, "mymkdir") == 0) {

		if (strcmp(ob2, "\0") || strcmp(ob1, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			//mymkdir();
		}

	}
	else if (strcmp(exam, "myrmdir") == 0) {

		if (strcmp(ob2, "\0") || strcmp(ob1, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			//myrmdir();
		}

	}
	else if (strcmp(exam, "myrm") == 0) {

		if (strcmp(ob2, "\0") || strcmp(ob1, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			//myrm();
		}

	}
	else if (strcmp(exam, "mymv") == 0) {

		if (strcmp(ob3, "\0") || strcmp(ob2, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			//mymv();
		}

	}
	else if (strcmp(exam, "mytouch") == 0) {

		if (strcmp(ob2, "\0") || strcmp(ob1, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			//mytouch();
		}

	}
	else if (strcmp(exam, "myinode") == 0) {

		if (strcmp(ob2, "\0") || strcmp(ob1, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			myinode(myfs, ob1);
		}

	}
	else if (strcmp(exam, "mydatablock") == 0) {

		if (strcmp(ob2, "\0") || strcmp(ob1, "\0") == 0) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			mydatablock(myfs, ob1);
		}

	}
	else if (strcmp(exam, "mystate") == 0) {

		if (strcmp(ob1, "\0")) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			mystate(myfs);
		}

	}
	else if (strcmp(exam, "mytree") == 0) {

		if (strcmp(ob2, "\0")) {
			printf("잘못된 입력입니다.\n");
		}
		else {
			//mytree();
		}

	}
	else if (strcmp(exam, "command") == 0) {


	}
	else if (strcmp(exam, "exit") == 0) {

		return 0;
	}

	return 1;
}
