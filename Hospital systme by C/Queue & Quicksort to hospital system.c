#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SWAP(x,y,t) ( (t)=(x), (x)=(y), (y)=(t) )//sorting을 하기 위한 swap
#define MAX_NAME_SIZE 50
#define MAX_GENDER 50
#define MAX_INFORMATION_SIZE 1000
#define MAX_SIZE 1000//환자 정보를 받을 구조체의 배열 크기

typedef struct{
	char name[MAX_NAME_SIZE];//환자 이름 입력 받을 char 배열
	char gender[MAX_GENDER];//환자의 성별을 입력 받을 char 배열
	char clinic_history[MAX_INFORMATION_SIZE];//환자의 병명을 입력 받을 char 배열
}element;//이 변수의 type을 element로 설정

typedef struct QueueNode{
   element item;
   struct QueueNode *link;
}QueueNode;//큐 노드(큐를 연결 list로 선언)

typedef struct {
   QueueNode *front, *rear;
}QueueType;//연결리스트로 선언된 큐의 시작과 끝을 나타냄

element list[MAX_SIZE];//방문한 환자를 배열로 저장. 즉, 환자의 database가 됨.(병원 list)
int num;//방문한 환자 수 표시

int compare(element e1, element e2){
	return strcmp(e1.name, e2.name);
}//입력 받은 이름의 알파벳에서 순서에 따라 1,0,-1을 나타냄

int partition(element list[], int left, int right){//quick sort를 하기 위한 정렬 과정 중 partition
	element pivot, temp;
	int low, high;//변수 선언
	low = left;//list의 가장 왼쪽
	high = right+1;//list의 가장 오른쪽
	pivot = list[left];//가장 왼쪽 변수가 기준

	do{
		do
			low++;
		while(compare(list[low], pivot)<0);//현재 low 부분의 환자 이름의 알파벳이 pivot보다 앞이면 -1을 return함(이 환자는 배열에서 자리를 옮길 필요 없음)
		do
			high--;
		while(compare(list[high], pivot)>0);//현재 high 부분의 환자 이름의 알파벳이 pivot보다 뒤면 1을 return함(이 환자 또한 배열에서 자리를 옮길 필요 없음)

		if(low<high) SWAP(list[low], list[high], temp);//각각 pivot자리를 기준으로 잘못된 위치에 있는 경우를 찾았으면 swap
	}while(low<high);//배열에서 low와 high의 위치가 바뀌기 전까지 수행
	SWAP(list[left], list[high], temp);//do-while문을 빠져 나와서 swap(배열의 가장 왼쪽에 있는 변수-pivot과 현재 high위치에 있는 변수를 서로 바꿈) 
	return high;//high값을 return
}

void quick_sort(element list[], int left, int right){//quick sort 수행
	if(left<right){//left가 더 크게 되면 quick sort 종료
		int q=partition(list, left, right);//partition에서 정렬되고 high값을 받음
		quick_sort(list, left, q-1);//순환적으로 quick sort 함수 호출(high의 앞부분 배열)
		quick_sort(list, q+1, right);//(high의 뒷부분 배열)
	}
}

void insert(element e){
	if(num < (MAX_SIZE)){
		list[num++] = e;
	}
}//환자의 정보를 배열에 입력하는 함수

void print(){
	int i;
	for(i=0; i<num; i++){
		printf("%s : %s, %s\n", list[i].name, list[i].gender, list[i].clinic_history);
	}
}//배열에 있는 환자를 모두 프린트

void error(char *message){
   fprintf(stderr, "%s\n", message);
   exit(1);
}//에러가 발생했을 경우 메세지 프린트하고 종료

void init(QueueType *q){
   q->front = q->rear = NULL;
}//큐를 초기화

int is_empty(QueueType *q){
   return (q->front == NULL);
}//큐가 비었는지에 대해서 return

void enqueue(QueueType *q, element item){//큐에 변수를 삽입하는 함수
   QueueNode *temp = (QueueNode *) malloc (sizeof (QueueNode));//큐 노드 생성
   if(temp == NULL)//temp가 할당되지 않았을 경우
      error("메모리를 할당할 수 없습니다.");
   else{
      temp->item = item;//환자 입력
      temp->link = NULL;//큐에서 마지막 노드이기 때문에 link가 가리키는 것은 없음
      if(is_empty(q)){//큐가 비어있는 경우
         q->front = temp;
         q->rear = temp;//front와 rear 변수가 모두 temp를 가리킴
      }
      else{//큐가 비어있지 않은 경우
         q->rear->link = temp;//큐의 rear가 가리키는 변수의 link가 temp를 가리킴
         q->rear = temp;//큐의 rear는 이제 가장 마지막인 temp를 가리킴
      }
   }
}

element dequeue(QueueType *q){//큐에서 변수 하나씩 빼는 함수
   QueueNode *temp = q->front;//큐에서 front가 가리키고 있는 노드를 temp가 가리킴
   element item;
   if(is_empty(q))//큐가 비었을 경우
      error("Error.");
   else{
      item = temp->item;//temp가 가리키는 노드를 item이 받음 
      q->front=q->front->link;//큐의 front는 원래 front가 가리키는 노드의 link가 가리키는 노드를 가리키게 됨
      if(q->front == NULL)//바뀐 front가 가리키는 곳이 없으면
         q->rear = NULL;//rear도 가리키는 곳 없음(즉, 큐가 비었음)
      free(temp);//할당한 temp 메모리에서 해제
      return item;//받은 item 노드를 return
   }
}

element peek(QueueType *q){//큐의 가장 앞에 있는 노드를 반환(큐에서 빼는 것은 아님)
   if(is_empty(q))//큐가 비었을 때는 error
      error("Error.");
   else 
      return q->front->item;//큐의 front가 가리키는 노드 반환
}

void add_clinic_history(element *patient){//환자가 재방문 했을 경우 진료 기록을 계속 추가해서 기록하도록 하는 함수 
	int i=0, count=0;
	char sick[50];//변수 선언
	while(patient->clinic_history[i++]!='\0'){//clinic_history 배열의 char가 끝나는 곳까지 이동
		count++;//count변수 증가(배열의 몇번째 index까지 char가 있는지 계산
	}
	patient->clinic_history[count++]=',';//다음 병명을 넣기 위해 comma 넣기
	patient->clinic_history[count++]=' ';//띄어 쓰기
	gets(sick);//병명 입력 받음
	i=0;//i를 다시 0으로 초기화
	while(sick[i]!='\0'){//입력받은 병명을 다시 clinic_history 배열로 옮김
		patient->clinic_history[count++]=sick[i++];//clinic_history는 앞에 입력된 char 다음 부터 입력 받고, sick은 배열 처음부터 char 넘겨줌
	}
	patient->clinic_history[count]='\0';//다 입력 받은 후에 끝에 string이 끝난다는 표시를 넣어줌
}

void menu(){//메뉴 프린트 함수
	printf("************************************\n");
	printf("1. Patient arrive at hospital\n");
	printf("2. Doctor Examine patient\n");
	printf("3. All patient list\n");
	printf("4. The hospital closed\n");
	printf("************************************\n");
}

void main(){
	FILE *fp;//파일에서 입력받기 위한 변수
   QueueType q;//큐 선언
   element patient;//환자 입력받기 위해 element 선언
   int i=0,n=0, information=0, run=1;
   char empty[10];//변수 선언

   init(&q);//큐 초기화
   fp = fopen("name_data.txt", "r");//파일 읽기 형식으로 받기


   do{//사용자가 원할 때까지 프로그램 실행
      menu();//메뉴 프린트
      scanf("%d", &n);//메뉴 선택

      switch(n){
      case 1://환자 병원 방문
		  fscanf(fp, "%s", patient.name);//파일로부터 환자이름 한 명 입력받기
		  if(feof(fp)){//파일에 끝에 도착한 경우에는 1번 메뉴 더이상 실행할 수 없음
			  printf("No more patient will visit hospital.\n");
			  break;
		  }
		  printf("%s visited the hospital.\n", patient.name);//누가 병원을 방문했는지 프린트
		  enqueue(&q, patient);//큐에 환자 넣음
         break;
      case 2://환자 진료
		  information=0;//병원을 방문한 적 있는지 확인한 후 setting하는 변수
		  if(is_empty(&q)){//큐가 비었을 경우 break
			  printf("There are no patient waiting for doctor.\n");
			  break;
		  }
		  patient = dequeue(&q);//대기 중인 환자를 한 명 부름(큐에서 꺼냄)
		  for(i=0; i<num; i++){//과거에 병원을 온 적 있는지 검사('list'에서 같은 이름이 있는지 비교)
			  if(strcmp(list[i].name,patient.name)==0){
				  information=1;//1로 set
				  break;//찾으면 더 이상 search하지 않고 break
			  }
		  }
		  if(information==1){//과거에 방문했는 경우
			   printf("%s : %s/ %s\n", list[i].name, list[i].gender, list[i].clinic_history);//환자 정보와 과거의 진료 기록 프린트
			   gets(empty);//buffer에서 enter를 받기 위해
			   puts("Why patient is sick?");
			   add_clinic_history(&list[i]);//환자 병명 입력
		  }
		  else{//과거에 방문하지 않은 경우
			  printf("%s is first visit.\n", patient.name);
			  printf("What is the patient's gender?\n");
			  scanf("%s", patient.gender);//환자 성별 입력
			  gets(empty);//buffer에 enter를 받기 위해
			  puts("Why patient is sick?");
			  gets(patient.clinic_history);//병명 입력
			  insert(patient);//list에 환자 추가
			  quick_sort(list,0,num-1);//quick sort로 알파벳 순으로 정렬
		  }
		  break;
      case 3://list에 있는 모든 환자의 정보 프린트
		  if(num==0){//list가 비어있는 경우 
			  printf("There are no patient information.\n");
			  break;
		  }
		  print();//모두 프린트하는 함수 호출
		  break;
      case 4://병원 문 닫음
		  printf("The hospital was closed\n");
		  while(!is_empty(&q)){//대기 중이던 환자들을 집으로 돌려보냄
			  patient = dequeue(&q);
			  printf("%s goes to home.\n", patient.name);
		  }
		  run=0;//프로그램 종료
		  break;
      default://다른 메뉴를 입력했을 때
         printf("Error(No such menu)\n");
		 break;
      }
	  printf("\n");
   }while(run!=0);
   fclose(fp);//file 닫기

}