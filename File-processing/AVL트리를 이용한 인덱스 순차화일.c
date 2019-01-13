#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#define MAX_COLS 32768
int f_num = 0;
// 노드의 구조
struct Record {
	int ID;
	char name[5];
	int age;
	int left;
	int right;
	int link;
};
int search_id;
char s[MAX_COLS];
struct Record record;
struct Record record2;
void insert_record(FILE *fp);
void insert_record2(FILE *fp2);
void search_record(FILE *fp);
int search_node2(FILE *fp2);
void delete_record(FILE *fp);
void delete_node2(FILE *fp2);
void change_record(FILE *fp);
void change_record2(FILE *fp2);
void term_m();
void record_printf(FILE *fp);
void menu();

void avl_search_record(FILE *fp);

//avl

struct avl_node {
	struct avl_node *left_child, *right_child;  /* Subtrees. */
	int data;                   /* Pointer to data. */
};



// 오른쪽 회전 함수
struct avl_node *
	rotate_right(struct avl_node *parent)
{
	struct avl_node *child = parent->left_child;
	parent->left_child = child->right_child;
	child->right_child = parent;
	return child;
}
// 왼쪽 회전 함수
struct avl_node *
	rotate_left(struct avl_node *parent)
{
	struct avl_node *child = parent->right_child;
	parent->right_child = child->left_child;
	child->left_child = parent;
	return child;
}
// 오른쪽-왼쪽 회전 함수
struct avl_node *
	rotate_right_left(struct avl_node *parent)
{
	struct avl_node *child = parent->right_child;
	parent->right_child = rotate_right(child);
	return rotate_left(parent);
}
// 왼쪽-오른쪽 회전 함수
struct avl_node *
	rotate_left_right(struct avl_node *parent)
{
	struct avl_node *child = parent->left_child;
	parent->left_child = rotate_left(child);
	return rotate_right(parent);
}
// 트리의 높이를 반환
int get_height(struct avl_node *node)
{
	int height = 0;
	if (node != NULL)
		height = 1 + max(get_height(node->left_child),
			get_height(node->right_child));
	return height;
}

// 노드의 균형인수를 반환
int get_height_diff(struct avl_node *node)
{
	if (node == NULL) return 0;
	return get_height(node->left_child) - get_height(node->right_child);
}

// 트리를 균형트리로 만든다
struct avl_node *
	rebalance(struct avl_node **node)
{
	int height_diff = get_height_diff(*node);
	if (height_diff > 1) {
		if (get_height_diff((*node)->left_child) > 0)
			*node = rotate_right(*node);
		else
			*node = rotate_left_right(*node);
	}
	else if (height_diff < -1) {
		if (get_height_diff((*node)->right_child) < 0)
			*node = rotate_left(*node);
		else
			*node = rotate_right_left(*node);
	}
	return *node;
}

// AVL 트리의 삽입 연산
struct avl_node *
	avl_add(struct avl_node **root, int new_key)
{
	if (*root == NULL) {
		*root = (struct avl_node *)
			malloc(sizeof(struct avl_node));
		if (*root == NULL) {
			fprintf(stderr, "메모리 할당 에러\n");
			exit(1);
		}
		(*root)->data = new_key;
		(*root)->left_child = (*root)->right_child = NULL;
	}
	else if (new_key < (*root)->data) {
		(*root)->left_child = avl_add(&((*root)->left_child),
			new_key);
		*root = rebalance(root);
	}
	else if (new_key >(*root)->data) {
		(*root)->right_child = avl_add(&((*root)->right_child), new_key);
		(*root) = rebalance(root);
	}
	else {
		fprintf(stderr, "중복된 키 에러\n");
		exit(1);
	}
	return *root;
}

// AVL 트리의 탐색함수
struct avl_node *avl_search(struct avl_node *node, int key)
{
	if (node == NULL) return NULL;
	printf("%d ", node->data);
	if (key == node->data) return node;
	else if (key < node->data)
		return avl_search(node->left_child, key);
	else
		return avl_search(node->right_child, key);
}



//--------------------------------------------

int get_height(FILE* fp, int r_num, int l_num);
int get_height_diff(FILE* fp);
FILE *fp;

char* timeToString(struct tm *t);


char* timeToString(struct tm *t) {
	static char s[20];

	sprintf(s, "%04d년%02d월%02d일 %02d시%02d분%02d초",
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec
	);

	return s;
}
struct avl_node *root;
int main()
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1300, 900, TRUE); // 800 width, 100 height
	SetConsoleTitle("T");

	char command;
	int key;
	FILE *fp;
	FILE *fp2;



	do {
		if ((fp = fopen("sequential.txt", "r+")) == NULL) {
			fp = fopen("sequential.txt", "w+");
			fprintf(fp, "%3d", -1);
		}
		if ((fp2 = fopen("tree.txt", "r+")) == NULL) {
			fp2 = fopen("tree.txt", "w+");
			fprintf(fp2, "%3d ", -1); // 초기화 
		}
		struct tm *t;
		time_t timer;

		timer = time(NULL);    // 현재 시각을 초 단위로 얻기
		t = localtime(&timer); // 초 단위의 시간을 분리하여 구조체에 넣기
		term_m();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t시작 시간 : %s\n", timeToString(t));
		menu();
		scanf("%c", &command);
		printf("\n\t\t\t━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n");
		system("cls");
		switch (command) {
		case '1':
			root = NULL;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			printf("\t\t\t┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
			printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t학생 정보를 삽입하는 모드입니다.\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t음수를 입력하면 종료합니다.\t\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t    ┃\n");
			printf("\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			printf("\t\t\t\t------- 학생의 정보는 다음과 같습니다 -------\n ");
			printf("\t\t\t\t아이디(3자리정수까지) 이름(5자리까지) 나이(3자리정수까지)\n\n");
			printf("\t\t\t\tID 이름 나이를 입력하세요");
			insert_record(fp);
			insert_record2(fp2);
			fclose(fp);
			fclose(fp2);
			if ((fp = fopen("sequential.txt", "r+")) == NULL) {
				fp = fopen("sequential.txt", "w+");
			}
			if (root != NULL)
				avl_search_record(fp);
			fclose(fp);
			Sleep(1000);

			break;

		case '2':
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
			printf("\t\t\t┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
			printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t학생 정보를 수정하는 모드입니다.\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t   입력을 완료하면 종료합니다.\t\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t    ┃\n");
			printf("\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			change_record(fp);
			change_record2(fp2);
			fclose(fp);
			fclose(fp2);
			break;

		case '3':
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			printf("\t\t\t┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
			printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t학생 정보를 삭제하는 모드입니다.\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t   입력을 완료하면 종료합니다.\t\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t    ┃\n");
			printf("\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			delete_record(fp);
			delete_node2(fp2);
			fclose(fp);
			fclose(fp2);
			break;

		case '4':
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
			printf("\t\t\t┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
			printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t학생 정보를 탐색하는 모드입니다.\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t   입력을 완료하면 종료합니다.\t\t\t\t\t    ┃\n");
			printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t    ┃\n");
			printf("\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			printf("\t\t\t\t탐색할 학생의 정보를 입력하세요 : ex) 1\n\n");
			printf("\t\t\t\t---- 학생의 ID를 입력하시면 됩니다. ----\n\n");
			printf("\t\t\t\t--> ");
			search_record(fp);
			search_node2(fp2);
			printf("\n\n\n\n");
			printf("\t\t\t\t\t\t5초 후에 초기 화면으로 돌아갑니다.\n\n");
			Sleep(900);
			printf("\t\t\t\t\t\t4초 후에 초기 화면으로 돌아갑니다.\n\n");
			Sleep(900);
			printf("\t\t\t\t\t\t3초 후에 초기 화면으로 돌아갑니다.\n\n");
			Sleep(900);
			printf("\t\t\t\t\t\t2초 후에 초기 화면으로 돌아갑니다.\n\n");
			Sleep(900);
			printf("\t\t\t\t\t\t1초 후에 초기 화면으로 돌아갑니다.\n\n");
			Sleep(500);
			fclose(fp);
			fclose(fp2);
			break;

		case '5':
			printf("\n\t\t\t\t\t********* 입력 받은 순서 *********\n");
			printf("\n\n\t\t\t\t");
			record_printf(fp);
			printf("\n\n");
			printf("\n\n\t\t\t\t\t******* Studen Information ******* \n\n");
			printf("\t\t\t\t\t\tID   이름 나이 링크필드 \n");
			if ((fp = fopen("sequential.txt", "rt")) == NULL) {
				fputs("Cannot open input file...\n", stderr);
				exit(1); // 모든 파일 닫고, 프로그램 종료
			}

			// 읽기 시작: 프로그램의 본체
			while (fgets(s, MAX_COLS, fp) != NULL) {
				printf("\n\n\t\t\t\t\t\t");
				printf(s); // 한 줄씩 화면에 출력
			}

			fcloseall(); // 모든 파일 닫기
			printf("\n\n\t\t\t\t\t");
			printf("학생의 정보는 10초 동안 출력됩니다.\n\n");
			Sleep(5000);
			printf("\t\t\t\t\t");
			printf("학생의 정보는 5초 동안 출력됩니다.\n\n");
			printf("\t\t\t\t\t출력 후에는 초기 화면으로 돌아갑니다.");
			Sleep(5000);

			break;
		case '6':
			printf("\t\t\t\t****** 트리로 구조와 내용을 나타냅니다. ******");
			break;
		case '7':
			//fseek(fp2, 0, SEEK_SET);
			//printf("%d", get_height(fp2,0,0));
			fseek(fp2, 0, SEEK_SET);
			printf("%d", get_height_diff(fp2));
			break;
		case 'q':
			printf("\n\n\n\n\n\n\n\t\t\t\t\t\t\t프로그램을 종료합니다. \n\n\n\t\t\t\t\t\t");
			break;
		default:
			printf("\t\t\t\t값을 다시 입력해주세요. 1~6까지만 해당됩니다. (나가기는 'Q')\n\n");
			break;
		}
	} while (command != 'q');
}
void insert_record(FILE *fp)
{
	int num = 0; int end = 99; int jduge;
	int prv; int overlap = 0;
	//if (fp == fseek(fp,0,SEEK_SET))
	//{
	//   fprintf(fp, "%03d %5s %03d 00\n", record.ID, record.name, record.age);
	//}
	int current_id;
	printf("잘못 들어오셨다면 -1 -1 -1을 입력해주세요\n\n");
	printf("\t\t\t\t--> ");
	scanf("%d %s %d", &record.ID, record.name, &record.age);
	record2 = record;
	if (record.ID == -1) {}
	else {
		fseek(fp, 0, SEEK_SET);
		fscanf(fp, "%3d", &current_id);
		if (current_id == -1) {
			fseek(fp, 0, SEEK_SET);
			fprintf(fp, "%03d %5s %03d %02d\n", record.ID, record.name, record.age, 01);
			avl_add(&root, record.ID);
		}
		else
		{
			fseek(fp, -4, SEEK_END);
			int find;
			while (1) {
				fscanf(fp, "%2d", &jduge);
				fseek(fp, -18, SEEK_CUR);
				fscanf(fp, "%3d", &find);

				if (find == record.ID) {
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
					printf("\n\n\t\t\t\t같은 ID가 있습니다. 다시 입력 하세요\n");
					printf("\n\t\t\t\t\t초기 화면으로 돌아갑니다. ");
					overlap = 1;
					break;
				}
				if (find < 0) {
					fseek(fp, 0, SEEK_END);
					break;
				}
				if (jduge == 1) {
					break;
				}
				fseek(fp, -7, SEEK_CUR);
			}
			if (overlap == 0) {
				fseek(fp, 0, SEEK_END);
				fprintf(fp, "%03d %5s %03d %02d\n", record.ID, record.name, record.age, end);
				fseek(fp, -40, SEEK_END);
				fscanf(fp, "%2d", &prv);
				if (find < 0) {}
				else {
					fseek(fp, -22, SEEK_END);
					fprintf(fp, "%02d\n", ++prv);
					fseek(fp, 0, SEEK_END);
				}
			}
			//      fseek(fp, -22, SEEK_END);
			//   fscanf(fp, "%02d", &jduge);
			//if (jduge == end) {
			//   fseek(fp, -22, SEEK_END);
			//   fprintf(fp, "%02d", i_num++);
			//}

		}
	}
	fclose(fp);
}

void insert_record2(FILE *fp2)
{
	int current_adr, parent_adr, new_adr;//자식은 현재, 부모는 부모노드
	struct Record parent, current = { -1, " ", -1, -1, -1 };
	current_adr = parent_adr = 0;
	parent.ID = NULL;

	while (1)
	{
		fseek(fp2, 21 * current_adr, SEEK_SET);
		fscanf(fp2, "%3d %5s %3d %2d %2d", &current.ID, current.name, &current.age, &current.left, &current.right);

		if (record2.ID == current.ID)
		{
			return;
		}
		if (record2.ID<current.ID&&current.left>0)
			current_adr = current.left;
		else if (record2.ID > current.ID&&current.right > 0)
			current_adr = current.right;
		else
			break;
	}
	parent_adr = current_adr;
	parent = current;
	if (parent.ID != -1)
	{
		fseek(fp2, 0, SEEK_END);
		new_adr = ftell(fp2);

		fprintf(fp2, "%03d %5s %3d %2d %2d\n", record2.ID, record2.name, record2.age, -1, -1);

		if (record2.ID < parent.ID) {
			parent.left = new_adr / 21;
			fseek(fp2, 21 * parent_adr + 14, SEEK_SET);
			fprintf(fp2, "%02d", parent.left);
		}
		else {
			parent.right = new_adr / 21;
			fseek(fp2, 21 * parent_adr + 17, SEEK_SET);;
			fprintf(fp2, "%02d", parent.right);
		}
	}
	else {
		fseek(fp2, 0, SEEK_SET);

		fprintf(fp2, "%03d %5s %3d %2d %2d\n", record2.ID, record2.name, record2.age, -1, -1);
		printf("\t\t\t\troot created!\n");
		Sleep(2000);

	}

}

void change_record(FILE *fp) {
	int change_age;
	int now_age;
	int find_id;
	int now_id;
	int now_index;
	int i = 0;

	printf("\t\t\t수정 하고자 하는 ID를 입력 하세요 \n\n\t\t\t--> ");
	scanf("%3d", &find_id);
	record2.ID = find_id;


	while (1) {
		fscanf(fp, "%03d", &now_id);
		fseek(fp, 10, SEEK_CUR);
		fscanf(fp, "%02d", &now_index);
		if (now_index == -1) {
			printf("삭제된 ID 입니다");
			break;
		}
		if (now_id == find_id) {
			fseek(fp, -6, SEEK_CUR);
			fscanf(fp, "%03d", &now_age);
			printf("\n\t\t\t\t현재 나이 : %d\n", now_age);
			printf("\t\t\t 수정하고자 하는 나이를 입력 하세요\n\n\t\t\t--> ");
			scanf("%d", &change_age);
			record2.link = change_age;

			fseek(fp, -3, SEEK_CUR);
			fprintf(fp, "%03d", change_age);

			break;
		}
	}
	fclose(fp);
	printf("\n\n\n\t\t\t\t\t\t수정 완료! \n\n");
	printf("\t\t\t\t\t\t5초 후에 초기 화면으로 돌아갑니다.\n\n");
	Sleep(900);
	printf("\t\t\t\t\t\t4초 후에 초기 화면으로 돌아갑니다.\n\n");
	Sleep(900);
	printf("\t\t\t\t\t\t3초 후에 초기 화면으로 돌아갑니다.\n\n");
	Sleep(900);
	printf("\t\t\t\t\t\t2초 후에 초기 화면으로 돌아갑니다.\n\n");
	Sleep(900);
	printf("\t\t\t\t\t\t1초 후에 초기 화면으로 돌아갑니다.\n\n");
	Sleep(500);

}
void change_record2(FILE *fp2) {
	int current_adr;
	struct Record parent, current = { -1, " ", -1, -1, -1 };
	current_adr = 0;
	while (1) {
		fseek(fp2, 21 * current_adr, SEEK_SET);
		fscanf(fp2, "%3d %5s %3d %2d %2d\n", &current.ID, current.name, &current.age, &current.left, &current.right);

		if (record2.ID == current.ID) {

			fseek(fp2, -11, SEEK_CUR);
			fprintf(fp2, "%03d", record2.link);

			break;

		}
		if (record2.ID < current.ID &&  current.left > 0) current_adr = current.left;
		else if (record2.ID > current.ID &&  current.right > 0) current_adr = current.right;
		else {
			printf("\t\t\t\t삭제된 ID 입니다.");
			break;
		}
	}
	fclose(fp2);
}

void menu() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf("\t*****************\t┏━━━━━━━━━━━━━━━━━━━━━━━━┓\t\t*******************\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	printf("\t* 담당교수 \t*\t┃  ┏━━━━━━━━━━━━━━━━━━━━━┓┃\t\t*   Developed by  *\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("\t* 이해각 교수님 *\t┃  ┃\t\t\t\t\t        ┃┃\t\t* 컴퓨터 공학과   *\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	printf("\t*****************\t┃  ┃     File Processing Term Project         ┃┃\t\t* 20134103 이상수 *\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	printf("\t\t\t\t┃  ┃                                          ┃┃\t\t* 20134087 이영재 *\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	printf("\t\t\t\t┃  ┗━━━━━━━━━━━━━━━━━━━━━┛┃\t\t* 20134080 김택윤 *\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	printf("\t\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━┛\t\t* 20134069 박지훈 *\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf("\t\t\t\t\t\t\t\t\t\t\t\t*******************\n\n\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf("\t\t\t┏━━━━━");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 159);
	printf(" 레코드에 대한 순차탐색과 직접탐색 모두 가능한 인덱스 순차화일  ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	printf("━━━━━┓\n ");
	printf("\t\t        ┃\t\t\t\t\t\t\t\t\t\t\t      ┃");
	printf("\t\n\t\t        ┃\t\t\t\t\t\t\t\t\t\t\t      ┃");
	printf("\t\n\t\t\t┃\t\t1. 학생 정보 삽입  2. 학생 정보 수정 3. 학생 정보 삭제\t\t\t      ┃ \n");
	printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t      ┃\n");
	printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t      ┃");
	printf("\t\n\t\t\t┃\t4. 학생 정보 탐색    5. 전체 학생 출력   6. 학생 트리전체 구조 및 정보\t      ┃\n ");
	printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t      ┃\n");
	printf("\t\t\t┃\t\t\t\t\t\t\t\t\t\t\t      ┃\n");
	printf("\t\t\t┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	printf("\t\t\t\t원하시는 모드를 선택하세요 : ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void term_m()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	printf("\n\n     ■■■■     ■■■     ■■■    ■          ■\n");
	printf("\t■        ■         ■  ■    ■■      ■■\n");
	printf("\t■        ■■■     ■■■    ■  ■   ■ ■\n");
	printf("\t■        ■         ■ ■     ■   ■ ■  ■\n");
	printf("\t■        ■■■     ■   ■   ■     ■   ■\n\n\n");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	printf("\t\t\t\t■■■    ■■■    ■■■■    ■■■       ■■    ■■■■\n");
	printf("\t\t\t\t■  ■    ■  ■       ■       ■         ■           ■\n");
	printf("\t\t\t\t■■■    ■■■       ■       ■■■     ■           ■\n");
	printf("\t\t\t\t■        ■ ■     ■ ■       ■         ■           ■\n");
	printf("\t\t\t\t■        ■   ■     ■        ■■■       ■■       ■\n\n");

}






//영재---------------------------------------

void search_record(FILE *fp)
{
	int num = 0;
	int index;


	scanf("%d", &search_id);
	record2.ID = search_id;
	fseek(fp, 0, SEEK_SET);


	printf("\n\n\t\t\t sequential : ");
	do
	{

		fscanf(fp, "%03d", &num);
		if (num == search_id)
		{

			printf("%03d 학생의 정보를 찾았습니다. ", num);
			break;

		}
		else
		{
			printf("%03d  ->  ", num);
			fseek(fp, 11, SEEK_CUR);
			fscanf(fp, "%02d", &index);

			fseek(fp, 0, SEEK_SET);
			fseek(fp, index * 18, SEEK_SET);
		}

	} while (index != 99);
	if (index == 99)
	{
		printf("탐색 결과가 없습니다 \n");
	}

}

int search_node2(FILE *fp2)
{
	int current_adr;
	struct Record parent, current = { -1," ",-1, -1, -1 };
	current_adr = 0;
	printf("\n\n\t\t\t\t tree : ");

	while (1) {
		fseek(fp2, 21 * current_adr, SEEK_SET);
		fscanf(fp2, "%3d %5s %3d %2d %2d\n", &current.ID, current.name, &current.age, &current.left, &current.right);

		if (record2.ID == current.ID) {
			printf("%03d 찾았습니다.\n", current.ID);

			return 0;
		}
		printf("%03d -> ", current.ID);
		if (record2.ID < current.ID &&  current.left > 0) current_adr = current.left;
		else if (record2.ID > current.ID &&  current.right > 0) current_adr = current.right;
		else {
			printf(" 탐색 결과가 없습니다!\n", record2.ID);

			break;
		}
	}

}

void delete_record(FILE *fp)
{
	int remove = -1;
	int prev_num, prev_index = -1;
	int current_num, current_index;
	int prevv_index;
	int check_c = -1;
	int flag = 0;

	printf("\t\t\t삭제 하고자 하는 ID를 입력 하세요 \n\n\t\t\t--> ");
	scanf("%d", &search_id);
	record2.ID = search_id;
	fseek(fp, 0, SEEK_SET);

	do {
		fscanf(fp, "%03d", &current_num);
		if (current_num == search_id)
		{
			if (prev_index == -1)
			{
				printf("\t\t\t\troot 노드를 삭제 합니다");
				fseek(fp, 11, SEEK_CUR);
				fprintf(fp, "%02d", remove);
				break;


			}
			else
			{
				fseek(fp, 11, SEEK_CUR);
				fscanf(fp, "%02d", &current_index);
				fseek(fp, -2, SEEK_CUR);
				fprintf(fp, "%02d", remove);
				while (1)
				{
					if (check_c != -1)
					{
						fseek(fp, -2, SEEK_CUR);
						fprintf(fp, "%02d", current_index);
						flag = 1;
						break;
					}
					fseek(fp, -20, SEEK_CUR);
					fscanf(fp, "%02d", &check_c);
				}


			}
		}
		else
		{
			fseek(fp, 11, SEEK_CUR);
			fscanf(fp, "%02d", &current_index);
			prev_index = current_index;
			fseek(fp, 0, SEEK_SET);
			fseek(fp, current_index * 18, SEEK_SET);
		}
		if (flag == 1)
			break;
	} while (1);


	printf("\t\t\t\t삭제 완료 !\n\n");
	printf("\t\t\t\t\t\t5초 후에 초기 화면으로 돌아갑니다.\n\n");
	Sleep(900);
	printf("\t\t\t\t\t\t4초 후에 초기 화면으로 돌아갑니다.\n\n");
	Sleep(900);
	printf("\t\t\t\t\t\t3초 후에 초기 화면으로 돌아갑니다.\n\n");
	Sleep(900);
	printf("\t\t\t\t\t\t2초 후에 초기 화면으로 돌아갑니다.\n\n");
	Sleep(900);
	printf("\t\t\t\t\t\t1초 후에 초기 화면으로 돌아갑니다.\n\n");
	Sleep(500);
}
void delete_node2(FILE *fp2)
{
	int current_adr, parent_adr, new_adr, succ_adr, succ_parent_adr, exist;
	char right_left;
	struct Record parent, current = { -1," ",-1, -1, -1 };
	struct Record succ, succ_parent = { -1," ",-1, -1, -1 };
	current_adr = parent_adr = 0;
	parent.ID = NULL;

	while (1) {
		fseek(fp2, 21 * current_adr, SEEK_SET);
		fscanf(fp2, "%3d %5s %3d %2d %2d", &current.ID, current.name, &current.age, &current.left, &current.right);

		if (record2.ID == current.ID) {
			exist = 1;
			break;
		}
		if (record2.ID < current.ID &&  current.left > 0) {
			parent_adr = current_adr;
			current_adr = current.left;
			right_left = 'L';
		}
		else if (record2.ID > current.ID &&  current.right > 0) {
			parent_adr = current_adr;
			current_adr = current.right;
			right_left = 'R';
		}
		else {
			exist = 0;
			break;
		}

	}

	if (exist == 0) {
		printf("Not Found!\n");
		return;
	}

	fseek(fp2, 21 * current_adr, SEEK_SET);
	fscanf(fp2, "%3d %5s %3d %2d %2d", &current.ID, current.name, &current.age, &current.left, &current.right);

	if (current.left == -1 && current.right == -1) {
		fseek(fp2, 21 * current_adr, SEEK_SET);
		fprintf(fp2, "%03d %5s %3d %2d %2d\n", -1, " ", -1, -1, -1);

		if (current_adr == 0) {
			return;
		}
		if (right_left == 'L') {
			fseek(fp2, 21 * parent_adr + 14, SEEK_SET);
			fprintf(fp2, "%02d", -1);
		}
		else {
			fseek(fp2, 21 * parent_adr + 17, SEEK_SET);
			fprintf(fp2, "%02d", -1);
		}
		return;
	}

	if (current.left > 0 && current.right == -1) {

		if (current_adr == 0) {
			fseek(fp2, 21 * current.left, SEEK_SET);
			fscanf(fp2, "%3d %5s %3d %2d %2d\n", &succ.ID, succ.name, &succ.age, &succ.left, &succ.right);
			fseek(fp2, 0, SEEK_SET);
			fprintf(fp2, "%03d %5s %3d %2d %2d\n", succ.ID, succ.name, succ.age, succ.left, succ.right);
			fseek(fp2, 21 * current.left, SEEK_SET);
			fprintf(fp2, "%03d %5s %3d %2d %2d\n", -1, " ", -1, -1, -1);
		}
		else {
			fseek(fp2, 21 * current_adr, SEEK_SET);
			fprintf(fp2, "%03d %5s %3d %2d %2d\n", -1, " ", -1, -1, -1);
			if (right_left == 'L') {
				fseek(fp2, 21 * parent_adr + 14, SEEK_SET);
				fprintf(fp2, "%02d", current.left);
			}
			else {
				fseek(fp2, 21 * parent_adr + 17, SEEK_SET);
				fprintf(fp2, "%02d", current.left);
			}
		}
		return;
	}

	if (current.left == -1 && current.right > 0) {
		if (current_adr == 0) {
			fseek(fp2, 21 * current.right, SEEK_SET);
			fscanf(fp2, "%3d %5s %3d %2d %2d\n", &succ.ID, succ.name, &succ.age, &succ.left, &succ.right);
			fseek(fp2, 0, SEEK_SET);
			fprintf(fp2, "%03d %5s %3d %2d %2d\n", succ.ID, succ.name, succ.age, succ.left, succ.right);
			fseek(fp2, 21 * current.right, SEEK_SET);
			fprintf(fp2, "%03d %5s %3d %2d %2d\n", -1, " ", -1, -1, -1);
		}
		else {

			fseek(fp2, 21 * current_adr, SEEK_SET);
			fprintf(fp2, "%03d %5s %3d %2d %2d\n", -1, " ", -1, -1, -1);
			if (right_left == 'L') {
				fseek(fp2, 21 * parent_adr + 14, SEEK_SET);
				fprintf(fp2, "%02d", current.right);
			}
			else {
				fseek(fp2, 21 * parent_adr + 17, SEEK_SET);
				fprintf(fp2, "%02d", current.right);
			}
		}
		return;
	}

	succ_adr = current.right;
	succ_parent_adr = current_adr;

	while (1) {
		fseek(fp2, 21 * succ_adr, SEEK_SET);
		fscanf(fp2, "%3d %5s %3d %2d %2d\n", &succ.ID, succ.name, &succ.age, &succ.left, &succ.right);
		if (succ.left == -1) {
			break;
		}
		succ_parent_adr = succ_adr;
		succ_adr = succ.left;
	}

	fseek(fp2, 21 * current_adr, SEEK_SET);
	fprintf(fp2, "%03d", succ.ID);

	if (succ_parent_adr == current_adr) {
		fseek(fp2, 21 * current_adr + 17, SEEK_SET);
		fprintf(fp2, "%02d", succ.right);
	}
	else {
		fseek(fp2, 21 * succ_parent_adr + 14, SEEK_SET);
		fprintf(fp2, "%02d", succ.right);
	}
	fseek(fp2, 21 * succ_adr, SEEK_SET);
	fprintf(fp2, "%03d %5s %3d %2d %2d\n", -1, " ", -1, -1, -1);
}


void record_printf(FILE *fp)
{
	int num = 0;
	int index;

	fseek(fp, 0, SEEK_SET);

	do
	{

		fscanf(fp, "%03d", &num);
		printf("%03d  ->  ", num);
		fseek(fp, 11, SEEK_CUR);
		fscanf(fp, "%02d", &index);

		fseek(fp, 0, SEEK_SET);
		fseek(fp, index * 18, SEEK_SET);

	} while (index != 99);
	if (index == 99)
	{
		printf("exit");
	}
}


//------------------avl
/*void rotate_right(FILE* fp)
{

}

void rebalance(FILE* fp)
{



}

int get_height_diff(FILE* fp)
{

}
*/




void avl_search_record(FILE *fp)
{
	int num = 0;
	int index;


	fseek(fp, 0, SEEK_SET);
	//printf("\n\n\t\t\t\t");
	do
	{


		fscanf(fp, "%03d", &num);
		avl_add(&root, num);

		//printf("%03d  ->  ", num);
		fseek(fp, 11, SEEK_CUR);
		fscanf(fp, "%02d", &index);

		fseek(fp, 0, SEEK_SET);
		fseek(fp, index * 18, SEEK_SET);

	} while (index != 99);



}
