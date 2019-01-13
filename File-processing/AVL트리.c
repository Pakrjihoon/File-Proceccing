#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

// 노드의 구조
struct TreeNode {
	int key;
	int left;
	int right;
};

void menu();
void insert_node(int key);
void search_node(int key);
void delete_node(int key);
int Get_height(int adr);
int Get_balance(int addr);
void rotate_LL(int addr);
void rotate_RR(int addr);
void rotate_RL(int addr);
void rotate_LR(int addr);
void AVL();

FILE *fp, *fd, *ct;
int root = 0;

// 이진 탐색 트리 프로그램 
void main() {
	char command; int  key;
	/*if ((ct = fopen("count.txt", "r+")) == NULL) {
	ct = fopen("count.txt", "w");
	}
	fscanf(ct,"%d", &count);*/
	// 화일을 읽기/쓰기 모드로 연다.(w+ 모드는 기존 내용을 지워버리므로 반드시 r+모드로..)
	// 그러나 r+모드는 화일이 존재하지 않을 경우 화일을 만들지 못하므로 다음과 같이 만들어준다. 
	if ((fp = fopen("tree.txt", "r+")) == NULL)
	{
		fp = fopen("tree.txt", "w+");
		fprintf(fp, "%2d %2d %2d\n", -1, -1, -1); // 초기화 
	}
	fd = fopen("tree.txt", "r+");
	do {
		menu();
		printf("현재 Root : %d \n", root);
		command = getchar();

		switch (command) {
		case 'i':  //삽입
			printf("음수를 입력하면 종료 \n");
			while (1) {
				printf("삽입키값:");
				scanf("%d", &key);
				if (key < 0) break;
				insert_node(key);
			}
			break;
		case 's':  //탐색
			printf("음수를 입력하면 종료\n");
			while (1) {
				printf("탐색키값:");
				scanf("%d", &key);
				if (key <0) break;
				search_node(key);
			}
			break;
		case 'd':  //삭제
			printf("음수를 입력하면 종료\n");
			while (1) {
				printf("삭제키값:");
				scanf("%d", &key);
				if (key <0) break;
				delete_node(key);
			}
			break;
		}
		fflush(stdin);
	} while (command != 'q');

}
// 삽입 함수
void insert_node(int key)
{
	int current_adr, parent_adr, new_adr; // current는 현재 노드, parent는 부모 노드 
	struct TreeNode parent, current = { -1, -1, -1 };
	int *Max_r, Max_l;
	current_adr = parent_adr = 0; int bal = 0;
	parent.key = NULL;
	while (1) {
		fseek(fp, 10 * current_adr, SEEK_SET);   //10 = 키 2자리 + 공백1 + 왼쪽 포인터 2 + 공백1 + 오른쪽 2 + CR+LF
		fscanf(fp, "%2d %2d %2d", &current.key, &current.left, &current.right);

		if (key == current.key) {
			printf("Duplicated Key!\n");
			return; //이미 존재하는 키값이면 종료
		}
		if (key < current.key &&  current.left > 0) current_adr = current.left;
		else if (key > current.key &&  current.right > 0) current_adr = current.right;
		else break;   //이 것은 leaf노드를 만났단 의미!

	}
	// 부모 노드와 링크 연결
	parent_adr = current_adr;
	parent = current;
	if (parent.key != -1)  //부모키값이 -1이라면 아무것도 없는 초기 상태임
	{
		fseek(fp, 0, SEEK_END);
		new_adr = ftell(fp);   //화일의 맨끝에 삽입하고 이 주소를 부모노드에 넘긴다
		fprintf(fp, "%02d %2d %2d\n", key, -1, -1);

		if (key < parent.key) {
			// 10으로 나누어서 절대 주소 
			parent.left = new_adr / 10;
			fseek(fp, 10 * parent_adr + 3, SEEK_SET);
			fprintf(fp, "%02d", parent.left);
			AVL();
		}
		else {
			// 10으로 나누어서 절대 주소
			parent.right = new_adr / 10;
			fseek(fp, 10 * parent_adr + 6, SEEK_SET);
			fprintf(fp, "%02d", parent.right);
			AVL();
		}
	}
	else {
		fseek(fp, 0, SEEK_SET);//        // 최초의 루트 삽입 
		fprintf(fp, "%02d %2d %2d\n", key, -1, -1); // 최초의 루트 삽입 
		printf("root created!\n");
	}
}
// 탐색 함수
void search_node(int key)
{
	int current_adr;
	struct TreeNode current = { -1, -1, -1 };
	current_adr = 0;
	while (1) {
		fseek(fp, 10 * current_adr, SEEK_SET);
		fscanf(fp, "%2d %2d %2d", &current.key, &current.left, &current.right);

		if (key == current.key) {
			printf("%d : Success !\n", key);
			return;
		}
		printf("%d -> ", current.key);
		if (key < current.key &&  current.left > 0) current_adr = current.left;
		else if (key > current.key &&  current.right > 0) current_adr = current.right;
		else {
			printf("%d : Failed\n", key);
			break;
		}
	}
}
// 삭제 함수
void delete_node(int key)
{
	int current_adr, parent_adr, find_adr, check_adr;
	struct TreeNode parent, current, find, check = { -1, -1, -1 };
	current_adr = parent_adr = find_adr = 0;
	parent.key = NULL;
	while (1) {
		fseek(fp, 10 * current_adr, SEEK_SET);
		fscanf(fp, "%2d %2d %2d", &current.key, &current.left, &current.right);
		fseek(fp, 10 * parent_adr, SEEK_SET);
		fscanf(fp, "%2d %2d %2d", &parent.key, &parent.left, &parent.right);

		// 탐색 완료
		if (key == current.key) {
			// 자식이 없다.
			if (current.left == -1 && current.right == -1) {
				fseek(fp, 10 * current_adr, SEEK_SET);
				fprintf(fp, "%2d %2d %2d", -1, -1, -1); // 삭제 표시
														// 삭제  후 처리
														// 부모와 자식 연결 끊어줌
				if (parent.key > current.key) {
					fseek(fp, 10 * parent_adr + 3, SEEK_SET);
					fprintf(fp, "%2d", -1);
				}
				else {
					fseek(fp, 10 * parent_adr + 6, SEEK_SET);
					fprintf(fp, "%2d", -1);
				}
			}
			// 자식이 하나 존재함
			else if (current.left == -1 || current.right == -1) {
				fseek(fp, 10 * current_adr, SEEK_SET);
				fprintf(fp, "%2d %2d %2d", -1, -1, -1); // 삭제 표시
														// 삭제 후 처리
														// 부모와 자식 연결 끊어줌 자식의 자식 위치 고려
				if (parent.key > current.key && current.right == -1) {
					fseek(fp, 10 * parent_adr + 3, SEEK_SET);
					fprintf(fp, "%02d", current.left);
				}
				else {
					fseek(fp, 10 * parent_adr + 6, SEEK_SET);
					fprintf(fp, "%02d", current.right);
				}
			}
			// 자식이 둘 존재함
			else { // current.left > 0 && current.right > 0
				fseek(fp, 10 * current_adr, SEEK_SET);
				fprintf(fp, "%2d %2d %2d", -1, -1, -1); // 삭제 표시
														// 삭제 후 처리 
				find_adr = current.right; // 오른쪽 자식으로 이동 
				fseek(fp, 10 * find_adr, SEEK_SET);
				fscanf(fp, "%2d %2d %2d", &find.key, &find.left, &find.right);
				if ((find.left == -1 && find.right == -1) ||
					(find.right > 0 && find.left == -1))
				{ // 오른쪽 자식이 아무 자식도없음 또는 오른쪽에 자식이 있음
					fseek(fp, 10 * find_adr + 3, SEEK_SET);
					fprintf(fp, "%02d", current.left);
				}
				else if (find.left > 0) { // 오른쪽 자식이 왼쪽에 자식이 있음 
										  // 왼쪽 자식중에 가장 작은 자식 찾음 = 왼쪽으로 이동 
					while (find.left > 0) {
						check_adr = find_adr; // 가장 작은 자식의 부모 노드 저장
						find_adr = find.left;
						fseek(fp, 10 * find_adr, SEEK_SET);
						fscanf(fp, "%2d %2d %2d", &find.key, &find.left, &find.right);
					}
					fseek(fp, 10 * find_adr + 3, SEEK_SET);
					// 가장 작은 노드 찾으면 그 노드가 지워진 노드 자리 차지함
					fprintf(fp, "%02d %02d", current.left, current.right);
					fseek(fp, 10 * check_adr, SEEK_SET);
					fscanf(fp, "%2d", &check.key);
					// 부모와 자식 연결 끊어줌 
					if (check.key > find.key && find.right > 0) {
						fseek(fp, 10 * check_adr + 3, SEEK_SET);
						fprintf(fp, "%02d", find.right);
					}
					else if (check.key > find.key && find.right == -1) {
						fseek(fp, 10 * check_adr + 3, SEEK_SET);
						fprintf(fp, "%02d", -1);
					}
				}
				// 루트를 삭제할때를 고려 
				if (current.key != parent.key) {
					// 부모와 자식 새로운 연결해줌 
					if (parent.key > find.key) {
						fseek(fp, 10 * parent_adr + 3, SEEK_SET);
						fprintf(fp, "%02d", find_adr);
					}
					else {
						fseek(fp, 10 * parent_adr + 6, SEEK_SET);
						fprintf(fp, "%02d", find_adr);
					}
				}
			}
		}
		// 부모노드의 주소를 저장할 임시 변수
		parent_adr = current_adr;
		if (key < current.key &&  current.left > 0) current_adr = current.left;
		else if (key > current.key &&  current.right > 0) current_adr = current.right;
		else {
			if (key != current.key) {
				printf("삭제하고자하는 %d 존재하지 않습니다.\n", key);
			}
			break;
		}
	}
}
void menu()
{
	printf("**************\n");
	printf("i: 입력\n");
	printf("d: 삭제\n");
	printf("s: 탐색\n");
	printf("q: 종료\n");
	printf("**************\n");
}
// 트리의 높이를 구하는 함수
int Get_height(int adr) {
	int r, l;
	struct TreeNode height_N = { -1,-1,-1 };
	fseek(fp, 10 * adr, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &height_N.key, &height_N.left, &height_N.right);
	if (adr != -1) {
		r = Get_height(height_N.right) + 1;
		l = Get_height(height_N.left) + 1;
		if (r > l) {
			return r;
		}
		else {
			return l;
		}
	}
	else {
		return 0;
	}
}
// 노드의 균형 인수 반환
int Get_balance(int addr) {
	struct TreeNode height = { -1, -1, -1 };
	fseek(fp, 10 * addr, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &height.key, &height.left, &height.right);
	return Get_height(height.left) - Get_height(height.right);
}
// RR 회전
void rotate_RR(int addr) {
	struct TreeNode head, child = { -1,-1,-1 };
	fseek(fp, 10 * addr, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &head.key, &head.left, &head.right);
	fseek(fp, 10 * head.right, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &child.key, &child.left, &child.right);
	fseek(fp, 10 * addr + 6, SEEK_SET);
	fprintf(fp, "%02d", child.left);
	fseek(fp, 10 * head.right + 3, SEEK_SET);
	fprintf(fp, "%02d", addr);
}
// LL 회전
void rotate_LL(int addr) {
	struct TreeNode head, child = { -1,-1,-1 };
	fseek(fp, 10 * addr, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &head.key, &head.left, &head.right);
	fseek(fp, 10 * head.left, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &child.key, &child.left, &child.right);
	fseek(fp, 10 * addr + 3, SEEK_SET);
	fprintf(fp, "%02d", child.right);
	fseek(fp, 10 * head.left + 6, SEEK_SET);
	fprintf(fp, "%02d", addr);
}
// RL 회전
void rotate_RL(int addr) {
	struct TreeNode head, child = { -1,-1,-1 };
	fseek(fp, 10 * addr, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &head.key, &head.left, &head.right);
	fseek(fp, 10 * head.left, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &child.key, &child.left, &child.right);
	rotate_LL(head.right);
	printf("addr : %d\n", addr);
	fseek(fp, 10 * addr + 6, SEEK_SET);
	fprintf(fp, "%02d", child.left);
	printf("addr : %d\n", addr);
	rotate_RR(addr);
}
// LR 회전
void rotate_LR(int addr) {
	struct TreeNode head, child = { -1, -1, -1 };
	fseek(fp, 10 * addr, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &head.key, &head.left, &head.right);
	fseek(fp, 10 * head.left, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &child.key, &child.left, &child.right);
	rotate_RR(head.left);
	fseek(fp, 10 * addr + 3, SEEK_SET);
	fprintf(fp, "%02d", child.right);
	rotate_LL(addr);
}
// 균형 유지
void AVL() {
	struct TreeNode child = { -1, -1, -1 };
	struct TreeNode find = { -1, -1, -1 };
	struct TreeNode check = { -1, -1, -1 };
	fseek(fp, 10 * root, SEEK_SET);
	fscanf(fp, "%2d %2d %2d", &child.key, &child.left, &child.right);
	// 왼쪽 서브트리의 균형을 맞춘다.
	if (Get_balance(root) > 1) {
		// LL
		if (Get_balance(child.left) > 0) {
			if (Get_balance(child.left) > 1) {
				fseek(fp, 10 * child.left, SEEK_SET);
				fscanf(fp, "%2d %2d %2d", &find.key, &find.left, &find.right);
				rotate_LL(child.left);
				printf("LL 균형 유지\n");
				fseek(fp, 10 * root + 3, SEEK_SET);
				fprintf(fp, "%02d", find.left);
			}
			else {
				rotate_LL(root);
				root = child.left;
				printf("LL 균형 유지\n");
			}
		}
		// LR
		else {
			if (Get_balance(child.left) < -1) {
				fseek(fp, 10 * child.left, SEEK_SET);
				fscanf(fp, "%2d %2d %2d", &find.key, &find.left, &find.right);
				fseek(fp, 10 * find.right, SEEK_SET);
				fscanf(fp, "%2d %2d %2d", &check.key, &check.left, &check.right);

				rotate_RL(child.left);
				fseek(fp, 10 * root + 3, SEEK_SET);
				fprintf(fp, "%02d", check.left);
				printf("RL 균형 유지\n");
			}
			else if (Get_balance(child.left) > 1) {
				rotate_LR(child.left);
				printf("LR 균형 유지\n");
			}
			else {
				fseek(fp, 10 * child.left, SEEK_SET);
				fscanf(fp, "%2d %2d %2d", &check.key, &check.left, &check.right);
				rotate_LR(root);
				root = check.right;
				printf("LR 균형 유지\n");
			}
		}

	}
	// 오른쪽 서브트리에 입력된 경우
	else if (Get_balance(root) < -1)
	{
		//  RR
		if (Get_balance(child.right) < 0) {
			if (Get_balance(child.right) < -1) {
				fseek(fp, 10 * child.right, SEEK_SET);
				fscanf(fp, "%2d %2d %2d", &find.key, &find.left, &find.right);
				rotate_RR(child.right);
				printf("RR 균형 유지\n");
				fseek(fp, 10 * root + 6, SEEK_SET);
				fprintf(fp, "%02d", find.right);
			}
			else {
				rotate_RR(root);
				root = child.right;
				printf("RR 균형 유지\n");

			}
		}
		// RL
		else {
			if (Get_balance(child.right))
				fseek(fp, 10 * child.right, SEEK_SET);
			fscanf(fp, "%2d %2d %2d", &check.key, &check.left, &check.right);
			rotate_RL(root);
			root = check.left;
			printf("RL 균형 유지\n");
		}
	}
	else {
		printf("균형 유지 안함 No critical\n");
	}
}
