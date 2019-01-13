/* This program is for run creation and also used in
external sorting later */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MEM_SIZE 5

void make_free();
void remain_buf();
void replace_key(int i);
void make_run_name(int run_number);
int mini();

FILE *fp, *fp_run;
int key[MEM_SIZE], count;
char fix[MEM_SIZE] = { ' ',' ',' ',' ',' ' };
char run_name[10];
void main()
{
	int i;
	fp = fopen("RUN-CRE.TXT", "r");
	//첫번째 run 화일 이름 만들기
	make_run_name(0);
	fp_run = fopen(run_name, "w");
	i = 0;
	while (1)
	{
		fscanf(fp, "%3d", &key[i]);
		if (key[i] <0) break;
		i++;
		if (i>MEM_SIZE - 1) break;
	}
	for (;;)
	{
		// 가장작은 key값 찾음
		i = mini();
		if (i < 0) // i = -1 key가 모두 동결되어 있음
		{
			make_free();
			i = mini();
		}
		replace_key(i);
	}
}

// key의 위치 변경
void replace_key(int i)
{
	int input, output;
	output = key[i];
	printf("%3d ", output);
	fprintf(fp_run, "%3d\n", output);
	fscanf(fp, "%3d", &input);
	key[i] = input;
	if (input < 0)
	{
		make_free();
		fix[i] = '*';
		remain_buf();
	}
	if (output > input) {
		fix[i] = '*';
	}
}

void make_free(void)
{
	int i;
	count++;
	for (i = 0; i <= MEM_SIZE - 1; i++) fix[i] = ' ';
	printf(" : %dth run\n", count);

	// 센티널 값(-1) 기록하고 런 생성 마감
	fprintf(fp_run, "%3d\n", -1);
	fclose(fp_run);

	// 새로운 run 화일 만들기
	make_run_name(count);
	fp_run = fopen(run_name, "w");
}
// key값의 최소값을 찾음
int mini()
{
	int min, min_arg, i;
	min = 999; // 초기 최소값
	min_arg = -1; // 전부다 동결되어 있다면 
	for (i = 0; i <= MEM_SIZE - 1; i++)
	{
		if (fix[i] != '*')
		{
			// 동결되어있지 않은 키중에서 
			if (key[i] < min)
			{
				min = key[i];
				min_arg = i;
			}
		}
	}
	return(min_arg);
}
// 남아있는 key 처리
void remain_buf()
{
	int i, j;
	for (i = 0; i <= MEM_SIZE - 2; i++)
	{
		j = mini();
		printf("%3d ", key[j]);
		fprintf(fp_run, "%3d\n", key[j]);
		fix[j] = '*';
	}
	printf(" : last run \n");
	// 센티널 값(-1) 기록하고 런 생성 마감
	fprintf(fp_run, "%3d\n", -1);
	fclose(fp_run);
	exit(1);
}

// 새로운 run 파일을 만듬 
void make_run_name(int run_number)
{
	char run_cnt[5];
	itoa(run_number, run_cnt, 10);
	strcpy(run_name, "run");
	strcat(run_name, run_cnt);
	strcat(run_name, ".txt");
}
