#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#define rows_lim 10
#define table_lim 10
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* TYPENAMES[] = { "None", "int", "char", "double", "float" ,"string"};

struct row {
	char name[20];
	int size;
	int type;
};

struct ex_col {
	struct row datas[rows_lim];
	char name[20];
};

struct table {
	void* loc;
	int mem_size;
	struct ex_col col_types;
	unsigned int count;
	char name[20];
};

//system func
struct table make_table(struct ex_col* datas);
struct table make_new_col(struct table* before);
void drop_table(struct table*);

//user interface
struct ex_col userApi_make_table_data();
void userApi_print_table(struct table* data);
void userApi_input_table(struct table* data, int input_loc);

//etc
int print_how();


void main()
{
	struct table tables[table_lim];
	for (int i = 0; i < table_lim; i++) {
		strcpy(tables[i].name, "TABLE_NOT_CREATED");
	}
	int mode;
	while (1) {
		mode = print_how();
		if (mode == 1) {
			printf("어느 주소에 새로운 테이블을 만들까요?\n");
			char tmp[10] = "none";
			for (int i = 0; i < table_lim; i++) {
				printf("[%d] %s\n", i, (tables[i].name != NULL) ? tables[i].name : tmp);
			}
			int num;
			printf("> ");
			scanf(" %d", &num);
			struct ex_col data = userApi_make_table_data();
			tables[num] = make_table(&data);
			printf("\n==<아무키나 눌러서 넘어가기>==\n");
			_getch();
		}
		else if (mode == 2) {
			printf("어떤 테이블을 원하시나요?\n");
			char tmp[10] = "none";
			for (int i = 0; i < 10; i++) {
				printf("[%d] %s\n", i, (tables[i].name != NULL) ? tables[i].name : tmp);
			}
			int num;
			printf("> ");
			scanf(" %d", &num);
			userApi_print_table(&(tables[num]));
			printf("\n==<아무키나 눌러서 넘어가기>==\n");
			_getch();
		}
		else if (mode == 3) {
			printf("어떤 테이블을 원하시나요?\n");
			char tmp[10] = "none";
			for (int i = 0; i < 10; i++) {
				printf("[%d] %s\n", i, (tables[i].name != NULL) ? tables[i].name : tmp);
			}
			int num;
			printf("> ");
			scanf(" %d", &num);
			make_new_col(&(tables[num]));
			userApi_input_table(&(tables[num]), tables[num].count);
			printf("\n==<아무키나 눌러서 넘어가기>==\n");
			_getch();
		}
		else if (mode == 4) {
			printf("어떤 테이블을 원하시나요?\n");
			char tmp[10] = "none";
			for (int i = 0; i < 10; i++) {
				printf("[%d] %s\n", i, (tables[i].name != NULL) ? tables[i].name : tmp);
			}
			int num;
			printf("> ");
			scanf(" %d", &num);
			drop_table(&(tables[num]));
			strcpy(tables[num].name, "TABLE_NOT_CREATED");
			printf("\n==<아무키나 눌러서 넘어가기>==\n");
			_getch();
		}
	}
}

struct table make_table(struct ex_col *datas)
{
	struct table result;
	strcpy(result.name, datas->name);
	int size_tot = 0;
	for (int i = 0; i < rows_lim; i++) {
		size_tot += (*datas).datas[i].size;
	}
	result.mem_size = size_tot;
	result.loc = NULL;
	result.count = 0;
	result.col_types = (*datas);
	return result;
}

struct table make_new_col(struct table* before)
{
	if ((*before).loc == NULL) {
		(*before).loc = malloc((*before).mem_size);
	}
	else {
		(*before).loc = realloc((*before).loc,(*before).mem_size*((*before).count+1));
	}
}

void drop_table(struct table* before)
{
	free((*before).loc);
}

struct ex_col userApi_make_table_data()
{
	struct ex_col result;
	int lim = rows_lim;
	int row_count = 0;

	printf("만드시려는 테이블의 이름을 입력해주세요 > ");
	scanf(" %s", &(result.name));
	printf("만드시려는 테이블의 열의 수를 입력해주세요(최대: %d)", lim);
	scanf(" %d", &row_count);


	for (int i = 0; i < row_count; i++) {
		struct row a;
		printf("%d번째 열입니다.\n", i + 1);
		printf("열 이름 > ");
		scanf(" %s", &a.name);
		printf("타입(1:int, 2:char, 3:float, 4:double, 5:string) > ");
		scanf(" %d", &a.type);
		if (a.type == 1) {
			a.size = sizeof(int);
		}
		else if (a.type == 2) {
			a.size = sizeof(char);
		}
		else if (a.type == 3) {
			a.size = sizeof(double);
		}
		else if (a.type == 4) {
			a.size = sizeof(float);
		}
		else if (a.type == 5) {
			int text_len=100;
			printf("최대 몇글자까지 이용하실 건가요? > ");
			scanf(" %d", &text_len);
			a.size = sizeof(char) * text_len;
		}
		result.datas[i] = a;
	}
	for (int i = row_count; i < rows_lim; i++) {
		struct row a;
		a.size = 0;
		result.datas[i] = a;
	}

	return result;
}

void userApi_print_table(struct table* data)
{
	void* print_ptr = (*data).loc;
	printf("테이블 이름 : %s\n", data->name);
	int count=0;
	for (int i = 0; i < rows_lim; i++)
	{
		if ((*data).col_types.datas[i].size != 0) {
			if ((*data).col_types.datas[i].type == 1) {
				printf("| %-20s ", (*data).col_types.datas[i].name);
				count += 23;
			}
			else if ((*data).col_types.datas[i].type == 2) {
				printf("| %-20s ", (*data).col_types.datas[i].name);
				count += 23;
			}
			else if ((*data).col_types.datas[i].type == 3) {
				printf("| %-20s ", (*data).col_types.datas[i].name);
				count += 23;
			}
			else if ((*data).col_types.datas[i].type == 4) {
				printf("| %-20s ", (*data).col_types.datas[i].name);
				count += 23;
			}
			else if ((*data).col_types.datas[i].type == 5) {
				char tmp[10];
				char numto[10];
				strcpy(tmp, "| %-");
				sprintf(numto, "%d", (*data).col_types.datas[i].size);
				strcat(tmp, numto);
				strcat(tmp, "s ");
				printf(tmp, (*data).col_types.datas[i].name);
				count += (*data).col_types.datas[i].size+3;
			}
		}
	}
	printf("|\n");
	for (int i = 0; i <= count; i++) {
		printf("-");
	}
	printf("\n");
	for (unsigned int i = 0; i < (*data).count; i++)
	{
		for (int j = 0; j < rows_lim; j++)
		{
			if ((*data).col_types.datas[j].size != 0) {
				if ((*data).col_types.datas[j].type == 1) {
					printf("| %-20d ", *((int*)print_ptr));
					((int*)print_ptr)++;
				}else if ((*data).col_types.datas[j].type == 2) {
					printf("| %-20c ", *((char*)print_ptr));
					((char*)print_ptr)++;
				}else if ((*data).col_types.datas[j].type == 3) {
					printf("| %-20lf ", *((double*)print_ptr));
					((double*)print_ptr)++;
				}
				else if ((*data).col_types.datas[j].type == 4) {
					printf("| %-20f ", *((float*)print_ptr));
					((float*)print_ptr)++;
				}else if ((*data).col_types.datas[j].type == 5) {
					char tmp[10];
					char numto[10];
					strcpy(tmp, "| %-");
					sprintf(numto, "%d", (*data).col_types.datas[j].size);
					strcat(tmp, numto);
					strcat(tmp, "s ");
					printf(tmp, ((char*)print_ptr));
					for (int k = 0; k < (*data).col_types.datas[j].size; k++) {
						((char*)print_ptr)++;
					}
				}
			}
		}
		printf("|\n");
	}
}

void userApi_input_table(struct table* data, int input_loc)
{
	void* real_writer = (void*)(((char*)(*data).loc) + input_loc*(*data).mem_size);
	for (int i = 0; i < rows_lim; i++)
	{
		if ((*data).col_types.datas[i].type == 1) {
			int temp;
			printf("%s열의 값(type:%s) > \n", (*data).col_types.datas[i].name, TYPENAMES[(*data).col_types.datas[i].type]);
			scanf(" %d", &temp);
			*((int*)real_writer) = temp;
			((int*)real_writer)++;
		}else if ((*data).col_types.datas[i].type == 2) {
			char temp;
			printf("%s열의 값(type:%s) > \n", (*data).col_types.datas[i].name, TYPENAMES[(*data).col_types.datas[i].type]);
			scanf(" %c", &temp);
			*((char*)real_writer) = temp;
			((char*)real_writer)++;
		}
		else if ((*data).col_types.datas[i].type == 3) {
			double temp;
			printf("%s열의 값(type:%s) > \n", (*data).col_types.datas[i].name, TYPENAMES[(*data).col_types.datas[i].type]);
			scanf(" %lf", &temp);
			*((double*)real_writer) = temp;
			((double*)real_writer)++;
		}
		else if ((*data).col_types.datas[i].type == 4) {
			float temp;
			printf("%s열의 값(type:%s) > \n", (*data).col_types.datas[i].name, TYPENAMES[(*data).col_types.datas[i].type]);
			scanf(" %f", &temp);
			*((float*)real_writer) = temp;
			((float*)real_writer)++;
		}
		else if ((*data).col_types.datas[i].type == 5) {
			char* temp = (char*)malloc((*data).col_types.datas[i].size);
			printf("%s열의 값(type:%s) > \n", (*data).col_types.datas[i].name, TYPENAMES[(*data).col_types.datas[i].type]);
			scanf(" %s", temp);
			strcpy((char*)real_writer, temp);
			for (int k = 0; k < (*data).col_types.datas[i].size; k++) {
				((char*)real_writer)++;
			}
		}
	}
	(*data).count++;
}

int print_how()
{
	int mode;
	system("cls");
	printf("**정보 프로젝트 - 간단한 DBMS 제작**\n");
	printf("---[명령 종류]-------------------------------------------\n");
	printf("| 1. 테이블 제작\t\t");
	printf("2. 테이블 읽기\t\t|\n");
	printf("| 3. 테이블 값 추가\t\t");
	printf("4. 테이블 삭제\t\t|\n");
	printf("---------------------------------------------------------\n");
	printf("명령 입력 > ");
	scanf(" %d", &mode);
	return mode;
}