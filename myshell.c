#include "myfs.h"

int main(void) {
	My_file_system_pointer myfs = make_file_system();
	Call_my_file_system(myfs);

	char* path=(char*)calloc(30,sizeof(char));
	strcpy(path, "/");
	char* str;
	char* exam;
	char* ob1;
	char* ob2;
	char* ob3;
	int boolExit = 1;

	while (boolExit) {
		str = (char*)calloc(30, sizeof(char));
		exam = (char*)calloc(10, sizeof(char));
		ob1 = (char*)calloc(7, sizeof(char));
		ob2 = (char*)calloc(7, sizeof(char));
		ob3 = (char*)calloc(7, sizeof(char));	
		printf("[%s ]$ ", path);
		scanf("%[^\n]", str);
		getchar();
		sscanf(str, "%s %s %s %s", exam, ob1, ob2, ob3);

		boolExit = commandCompare(myfs, exam, ob1, ob2, ob3, path);

		free(str);
		free(exam);
		free(ob1);
		free(ob2);
		free(ob3);
		Save_my_file_system(myfs);
	}

	return 0;
}

