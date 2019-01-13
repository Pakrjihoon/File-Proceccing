/* 문자열 Key값에 대한 Hashing의 예
1. 알파벳을 숫자로 변환 : toupper(data.key[i])-64 (대소문자 동일하게)

   A  B  C  D  E  F  G  H  I  J   K  L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z
   1  2  3  4  5  6  7  8  9 10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26

2. 각 문자에 대한 xor 연산을 해준다 
		for(i=0; i<strlen(new_input); i++)           
			addr = addr^(toupper(new_input[i])-64);

    예) AND 의 경우 1 XOR 14 XOR 4
				=    0001(2)
				 XOR 1110(2)
				 XOR 0100(2)
				 ==========
				     1011(2) = 11  ---> Hash된 주소값
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUMBER_OF_BUCKETS 32
#define NUMBER_OF_OVERFLOWS 21
#define BLOCK_SIZE 18   //데이터 14 + 공백문자 2 + crlf 2



int main(void)
{
	FILE *fp;
	char  new_input[11], key[11];
	int i, link, new_link, addr, home, location;

	// 성적 파일을 읽기/쓰기 모드로 연다.(w+ 모드는 기존 내용을 지워버리므로 반드시 r+모드로..)
	// 그러나 r+모드는 화일이 존재하지 않을 경우 화일을 만들지 못하므로 다음과 같이 만들어준다. 

	if( (fp = fopen("word.txt", "r+")) == NULL )
	{
		fp = fopen("word.txt", "w");
		for (i=0;i<NUMBER_OF_BUCKETS+NUMBER_OF_OVERFLOWS;i++)
		{
			fseek(fp, i*BLOCK_SIZE, SEEK_SET);

				fprintf(fp, "%02d %10s %2d\n",i, "__________", -1); // 초기화 

		}
				fprintf(stdout,"화일이 존재하지 않아 새로 만들었습니다.\n");
				fclose(fp);
				fp = fopen("word.txt", "r+");
	}

	// 사용자로부터 학번, 이름, 성적을 입력받아서 파일에 저장한다.
	while(1)
	{
		addr=0;
		printf("키값을 입력하시요: (X이면 종료)");
 		fflush(stdin);
		scanf("%s", new_input);
		if(strcmp(new_input, "X")==0 || strcmp(new_input, "x")==0) 	break;

		for(i=0; i<strlen(new_input); i++)           
			addr = addr^(toupper(new_input[i])-64);   //각 문자열에 대한 XOR 연산을 수행함

		fseek(fp, addr*BLOCK_SIZE, SEEK_SET);
		fscanf(fp, "%d %s %d", &location, key, &link);

		if(strcmp(key, "__________") == 0)
		{
		fprintf(stdout, "삽입 : %d\n", location);
			fseek(fp, addr*BLOCK_SIZE + 3, SEEK_SET);
			fprintf(fp, "%10s %d\n", new_input, -1);
		}
		else
		{
			home = addr;
			new_link=link;    //새로 삽입될 키값의 링크는 홈버켓의 링크값을 사용함

			addr = NUMBER_OF_BUCKETS;   //오버플로우 영역 탐색
			do 
			{
				fseek(fp, addr*BLOCK_SIZE, SEEK_SET);
				fscanf(fp, "%d %s %d", &location, key, &link);
				addr ++;
			} while (strcmp(key, "__________") != 0);

			addr--;   // do while 문장이므로 추가 증가분 빼줌

			//오버플로우 영역에 기록
//
//
//
			//기본구역 링크 수정
//
//
		}
	
	}
	fclose(fp);
	return 0;

} 






#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUMBER_OF_BUCKET 33
#define NUMBER_OF_OVERFLOWS 21
#define BLOCK_SIZE 18   //데이터 14 + 공백문자 2 + crlf 2

int main(void)
{
	FILE *fp;
	char  new_input[11], key[11];
	int i, link, new_link, addr, home, location;

	// 사용자로부터 학번, 이름, 성적을 입력받아서 파일에서 탬색을 시작한다.
	while(1)
	{
		addr=0;
		printf("키값을 입력하시요: (X이면 종료)");
		scanf("%s", new_input);
		if(strcmp(new_input, "X")==0 || strcmp(new_input, "x")==0) break;

		for(i=0; i<strlen(new_input); i++)           
			addr = addr^(toupper(new_input[i])-64);   //각 문자열에 대한 XOR 연산을 수행함

		fp = fopen("word.txt", "r+");
		while(1)
		{
		fseek(fp, addr*BLOCK_SIZE, SEEK_SET);
		fscanf(fp, "%d %s %d", &location, key, &link);
			if(strcmp(key,new_input)==0)   //입력값과 같은 레코드 키를 찾으면
			{
				printf("찾은 주소 = %d\n", addr);
				fclose(fp);
				break;
			}
			else
			{
				if (link == -1)    // 링크(체인)의 끝을 만나면
				{
					printf("찾을 수 없습니다.\n");
					break;
				}
				else
				{
				     printf("%d -> ", addr);
					addr = link;  // 다음 링크를 따라 탐색 계속한다.
					continue;
				}
			}
		}

	}
fclose(fp);
return 0;

} 

