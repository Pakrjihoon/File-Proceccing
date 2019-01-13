#include <stdio.h>
#include <stdlib.h>

#define DIVIDER 7

struct student {
	int number;
	char name[10];
	int score;
};

struct student data;

int linear_probe(int number);
void find_num(int finds);
void delete_num(int del);

int main(void)
{
	FILE *fp; int addr;
	int i; int op; int find; int dele;

	// 성적 파일을 읽기/쓰기 모드로 연다.(w+ 모드는 기존 내용을 지워버리므로 반드시 r+모드로..)
	// 그러나 r+모드는 화일이 존재하지 않을 경우 화일을 만들지 못하므로 다음과 같이 만들어준다. 
	if ((fp = fopen("score.txt", "r+")) == NULL)
	{
		fp = fopen("score.txt", "w");
		for (i = 0; i<DIVIDER; i++)
		{
			fseek(fp, i*(16 + 2), SEEK_SET);  //16 = 2(number) + 1(space) + 10(name) + 1(space) + 2(score), 여기에 CRLF 2 추가
			fprintf(fp, "%2d %10s %2d\n", -1, "----------", -1); // 초기화 
		}  fclose(fp);
		fprintf(stderr, "화일이 존재하지 않아 새로 만들었습니다.\n");
	}
	printf("모드를 입력하세요. ( 1: 입력, 2: 찾기, 3:삭제) : ");
	scanf("%d", &op);
	switch (op)
	{
	case 1:
		while (1)
		{
			printf("학번, 이름, 성적을 입력하시요: (음수이면 종료)");
			scanf("%d", &data.number);
			if (data.number < 0) break;
			scanf("%s %d", data.name, &data.score);
			addr = linear_probe(data.number);  //선형탐색 시작

			switch (addr)  //반환된 주소값에 따라 처리
			{
			case -1: //키값이 이미 존재
				printf("키값이 이미 존재합니다\n");
				fclose(fp);
				break;
			case -2:  //빈공간이 없음
				printf("레코드를 추가할 수 없습니다\n");
				fclose(fp);
				break;
			default:
				fseek(fp, addr * 18, SEEK_SET);
				fprintf(fp, "%2d %10s %2d", data.number, data.name, data.score);
				printf("%d 번지에 저장하였습니다.\n", addr);
				fclose(fp);
				break;
			}
		}
		break;
	case 2:
		while (1) {
			printf("찾고자하는 학번을 입력하시요: (음수이면 종료)");
			scanf("%d", &find);
			if (find < 0) break; // 음수이면 종료
			find_num(find);
		}
		break;
	case 3:
		while (1) {
			printf("제거하고자하는 학번을 입력하시요: (음수이면 종료)");
			scanf("%d", &dele);
			if (dele < 0) break; // 음수이면 종료
			delete_num(dele);
		}
		break;
	}
	// 사용자로부터 학번, 이름, 성적을 입력받아서 파일에 저장한다.


	return 0;
}

int linear_probe(int number)
{
	FILE *fp;
	int bucket, home_addr, exist;
	bucket = number % DIVIDER;
	home_addr = bucket;
	fp = fopen("score.txt", "r+");
	fseek(fp, bucket * 18, SEEK_SET);
	fscanf(fp, "%d", &exist);  //기존에 있는 데이터의 키값을 읽어봄

	while (exist>0) // 버켓에 레코드가 존재하면 다음 버켓 탐색을 계속함
	{
		// 입력하는 키값과 기존 키값 동일
		if (number == exist) return -1; // 키값이 이미 존재
		bucket = (bucket + 1) % DIVIDER;
		// 빈공간을 탐색하다가 처음의 위치로 돌아옴
		if (bucket == home_addr) return -2; // 빈공간이 존재하지 않음
											// 다음 공간으로 넘어간다.
		fseek(fp, bucket * 18, SEEK_SET);
		fscanf(fp, "%d", &exist);
	}
	return bucket;
}
void find_num(int finds) {
	int find_addr; int home;
	FILE *fp; fp = fopen("score.txt", "r+");
	find_addr = finds % DIVIDER;
	home = find_addr-1;
	while (home != find_addr) {
		fseek(fp, find_addr * 18, SEEK_SET);
		fscanf(fp, "%2d %10s %2d", &data.number, data.name, &data.score);
		find_addr = (find_addr + 1) % DIVIDER;
		if (data.number == finds) {
			printf("%s %d\n", data.name, data.score);
			break;
		}
	}
}
void delete_num(int del) {
	int dele_addr; int home; int pre_addr; int succ = 0;
	FILE *fp; fp = fopen("score.txt","r+");
	dele_addr = del % DIVIDER;
	home = dele_addr-1;
	while (home != dele_addr) {
		fseek(fp, dele_addr * 18, SEEK_SET);
		fscanf(fp, "%2d %10s %2d", &data.number, data.name, &data.score);
		pre_addr = dele_addr;
		dele_addr = (dele_addr + 1) % DIVIDER;
		if (data.number == del) {
			fseek(fp, pre_addr * 18, SEEK_SET);
			fprintf(fp,"%2d %10s %2d", -1, "----------",-1);
			printf("%d를 삭제 완료하였습니다.\n", del);
			succ = 1;
			break;
		}
	}
	if ( succ == 0)
		printf("삭제하고자하는 %d가 존재하지 않습니다.\n", del);
}
