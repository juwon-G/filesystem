#ifndef myfsheader
#define myfsheader

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>


#define MAX_BLOCK_SIZE 257		// per byte
#define MAX_DATA_BLOCK_COUNT 256
#define MAX_INODE_COUNT 128
#define MAX_INODE_BLOCK_CAPACITY 8

#endif

#pragma pack(push, 1)			// struct memory padding

typedef struct date             // 날짜
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
}Date;
typedef Date* Date_pointer;

typedef struct data_block	// max_size: 256byte
{
	char* data;
	int data_size;
} Data_block;
typedef Data_block* Data_block_pointer;

typedef struct data_block_list	// max_point_capacity : 8
{
	Data_block_pointer data_blocks;
	int block_count;
} data_block_list;
typedef data_block_list* Data_block_list;

typedef struct indirect_block	// max_point_capacity : 8
{
	Data_block_list D_block_list;
	int data_count;
}Indirect_block;
typedef Indirect_block* Indirect_block_pointer;

typedef struct inode
{
	int node_type;					// 파일의 타입 ( head
	Date_pointer create_time;				// 파일의 생성 날짜 (년/월/일 시:분:초)
	int node_size;							// file_size (total byte: direct + indirect) [ .data_size ]
	char* node_name;						// max_size: 8(ascii), 16(UTF-8)
	int* D_block;
	int indir_block;

} Inode;
typedef Inode* Inode_pointer;

typedef struct
{
	unsigned
		b00 : 1, b01 : 1, b02 : 1, b03 : 1, b04 : 1, b05 : 1, b06 : 1, b07 : 1,
		b08 : 1, b09 : 1, b10 : 1, b11 : 1, b12 : 1, b13 : 1, b14 : 1, b15 : 1;
} Boot_bit_map;

typedef struct
{
	unsigned* Super_block_inode;
	/*
	unsigned
		b000 : 1, b001 : 1, b002 : 1, b003 : 1, b004 : 1, b005 : 1, b006 : 1, b007 : 1,
		b008 : 1, b009 : 1, b010 : 1, b011 : 1, b012 : 1, b013 : 1, b014 : 1, b015 : 1,
		b016 : 1, b017 : 1, b018 : 1, b019 : 1, b020 : 1, b021 : 1, b022 : 1, b023 : 1,
		b024 : 1, b025 : 1, b026 : 1, b027 : 1, b028 : 1, b029 : 1, b030 : 1, b031 : 1,
		b032 : 1, b033 : 1, b034 : 1, b035 : 1, b036 : 1, b037 : 1, b038 : 1, b039 : 1,
		b040 : 1, b041 : 1, b042 : 1, b043 : 1, b044 : 1, b045 : 1, b046 : 1, b047 : 1,
		b048 : 1, b049 : 1, b050 : 1, b051 : 1, b052 : 1, b053 : 1, b054 : 1, b055 : 1,
		b056 : 1, b057 : 1, b058 : 1, b059 : 1, b060 : 1, b061 : 1, b062 : 1, b063 : 1,
		b064 : 1, b065 : 1, b066 : 1, b067 : 1, b068 : 1, b069 : 1, b070 : 1, b071 : 1,
		b072 : 1, b073 : 1, b074 : 1, b075 : 1, b076 : 1, b077 : 1, b078 : 1, b079 : 1,
		b080 : 1, b081 : 1, b082 : 1, b083 : 1, b084 : 1, b085 : 1, b086 : 1, b087 : 1,
		b088 : 1, b089 : 1, b090 : 1, b091 : 1, b092 : 1, b093 : 1, b094 : 1, b095 : 1,
		b096 : 1, b097 : 1, b098 : 1, b099 : 1, b100 : 1, b101 : 1, b102 : 1, b103 : 1,
		b104 : 1, b105 : 1, b106 : 1, b107 : 1, b108 : 1, b109 : 1, b110 : 1, b111 : 1,
		b112 : 1, b113 : 1, b114 : 1, b115 : 1, b116 : 1, b117 : 1, b118 : 1, b119 : 1,
		b120 : 1, b121 : 1, b122 : 1, b123 : 1, b124 : 1, b125 : 1, b126 : 1, b127 : 1;
		*/
} Inode_bit_map;
typedef Inode_bit_map* Inode_bit_map_pointer;

typedef struct
{
	unsigned* Super_block_data_block;
	/*
	unsigned
		b000 : 1, b001 : 1, b002 : 1, b003 : 1, b004 : 1, b005 : 1, b006 : 1, b007 : 1,
		b008 : 1, b009 : 1, b010 : 1, b011 : 1, b012 : 1, b013 : 1, b014 : 1, b015 : 1,
		b016 : 1, b017 : 1, b018 : 1, b019 : 1, b020 : 1, b021 : 1, b022 : 1, b023 : 1,
		b024 : 1, b025 : 1, b026 : 1, b027 : 1, b028 : 1, b029 : 1, b030 : 1, b031 : 1,
		b032 : 1, b033 : 1, b034 : 1, b035 : 1, b036 : 1, b037 : 1, b038 : 1, b039 : 1,
		b040 : 1, b041 : 1, b042 : 1, b043 : 1, b044 : 1, b045 : 1, b046 : 1, b047 : 1,
		b048 : 1, b049 : 1, b050 : 1, b051 : 1, b052 : 1, b053 : 1, b054 : 1, b055 : 1,
		b056 : 1, b057 : 1, b058 : 1, b059 : 1, b060 : 1, b061 : 1, b062 : 1, b063 : 1,
		b064 : 1, b065 : 1, b066 : 1, b067 : 1, b068 : 1, b069 : 1, b070 : 1, b071 : 1,
		b072 : 1, b073 : 1, b074 : 1, b075 : 1, b076 : 1, b077 : 1, b078 : 1, b079 : 1,
		b080 : 1, b081 : 1, b082 : 1, b083 : 1, b084 : 1, b085 : 1, b086 : 1, b087 : 1,
		b088 : 1, b089 : 1, b090 : 1, b091 : 1, b092 : 1, b093 : 1, b094 : 1, b095 : 1,
		b096 : 1, b097 : 1, b098 : 1, b099 : 1, b100 : 1, b101 : 1, b102 : 1, b103 : 1,
		b104 : 1, b105 : 1, b106 : 1, b107 : 1, b108 : 1, b109 : 1, b110 : 1, b111 : 1,
		b112 : 1, b113 : 1, b114 : 1, b115 : 1, b116 : 1, b117 : 1, b118 : 1, b119 : 1,
		b120 : 1, b121 : 1, b122 : 1, b123 : 1, b124 : 1, b125 : 1, b126 : 1, b127 : 1,
		b128 : 1, b129 : 1, b130 : 1, b131 : 1, b132 : 1, b133 : 1, b134 : 1, b135 : 1,
		b136 : 1, b137 : 1, b138 : 1, b139 : 1, b140 : 1, b141 : 1, b142 : 1, b143 : 1,
		b144 : 1, b145 : 1, b146 : 1, b147 : 1, b148 : 1, b149 : 1, b150 : 1, b151 : 1,
		b152 : 1, b153 : 1, b154 : 1, b155 : 1, b156 : 1, b157 : 1, b158 : 1, b159 : 1,
		b160 : 1, b161 : 1, b162 : 1, b163 : 1, b164 : 1, b165 : 1, b166 : 1, b167 : 1,
		b168 : 1, b169 : 1, b170 : 1, b171 : 1, b172 : 1, b173 : 1, b174 : 1, b175 : 1,
		b176 : 1, b177 : 1, b178 : 1, b179 : 1, b180 : 1, b181 : 1, b182 : 1, b183 : 1,
		b184 : 1, b185 : 1, b186 : 1, b187 : 1, b188 : 1, b189 : 1, b190 : 1, b191 : 1,
		b192 : 1, b193 : 1, b194 : 1, b195 : 1, b196 : 1, b197 : 1, b198 : 1, b199 : 1,
		b200 : 1, b201 : 1, b202 : 1, b203 : 1, b204 : 1, b205 : 1, b206 : 1, b207 : 1,
		b208 : 1, b209 : 1, b210 : 1, b211 : 1, b212 : 1, b213 : 1, b214 : 1, b215 : 1,
		b216 : 1, b217 : 1, b218 : 1, b219 : 1, b220 : 1, b221 : 1, b222 : 1, b223 : 1,
		b224 : 1, b225 : 1, b226 : 1, b227 : 1, b228 : 1, b229 : 1, b230 : 1, b231 : 1,
		b232 : 1, b233 : 1, b234 : 1, b235 : 1, b236 : 1, b237 : 1, b238 : 1, b239 : 1,
		b240 : 1, b241 : 1, b242 : 1, b243 : 1, b244 : 1, b245 : 1, b246 : 1, b247 : 1,
		b248 : 1, b249 : 1, b250 : 1, b251 : 1, b252 : 1, b253 : 1, b254 : 1, b255 : 1;
		*/
} Data_block_bit_map;
typedef Data_block_bit_map* Data_block_bit_map_pointer;

typedef struct
{
	Inode_bit_map_pointer I_bit_map;			// Inode bit map
	Data_block_bit_map_pointer D_bit_map;		// data block bit map
} Super_block;
typedef Super_block* Super_block_pointer;

typedef struct
{
	Boot_bit_map B_bit_map;

	Super_block_pointer S_block;

	Inode_pointer I_list;				// Inode list

	Data_block_list D_list;				// data block
} My_file_system;
typedef My_file_system* My_file_system_pointer;

#pragma pack(pop)

//makestruct
void set_date(Date_pointer);
Super_block_pointer make_super_block(void);
char* make_data_block(void);
Data_block_pointer make_data_blocks(void);
Data_block_list make_data_block_list(void);
Inode_pointer make_inode_list(void);
My_file_system_pointer make_file_system(void);
//func
void Call_my_file_system(My_file_system_pointer myfs);
void Save_my_file_system(My_file_system_pointer myfs);
int char_count(char* str, char c);
int Search_inode(My_file_system_pointer myfs, char* ob1, char* path);
void bit_print(unsigned a);
int comp_voca(const void* p, const void* q);
int commandCompare(My_file_system_pointer myfs, char* exam, char* ob1, char* ob2, char* ob3, char* path);
//commands
void mymkfs(My_file_system_pointer myfs);
void myls(My_file_system_pointer myfs, char* ob1, char* path);
void mycat(My_file_system_pointer myfs, char* ob1, char* path);
void myshowfile(My_file_system_pointer myfs, char* ob1, char* ob2, char* ob3, char* path);
void mypwd(char* path);
void mycpto(My_file_system_pointer myfs, char* ob1, char* ob2, char* path);
void mycpfrom(My_file_system_pointer myfs, char* ob1, char* ob2, char* path);
void myinode(My_file_system_pointer myfs, char* ob1);
void mydatablock(My_file_system_pointer myfs, char* ob1);
void mystate(My_file_system_pointer myfs);
