/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4  // 큐의 최대 크기=4로 정의

typedef char element;  // 큐에 저장될 요소의 타입 정의
typedef struct {
    element queue[MAX_QUEUE_SIZE];  // 큐의 요소를 저장하는 배열 선언
    int front, rear;  // 큐의 front와 rear 위치를 나타내는 변수
}QueueType;

// 함수 선언 파트
QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
    QueueType *cQ = createQueue();  // 큐를 생성
    element data;  // 큐에 삽입할 데이터를 저장할 변수
    char command;  // 사용자가 입력하는 명령을 입력 입력받는 변수

    do{
        printf("[----- [박조현] [2021053017] -----]\n");
        printf("\n-----------------------------------------------------\n");
        printf("                     Circular Q                   \n");
        printf("------------------------------------------------------\n");
        printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
        printf("------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command);

        switch(command) {  // 사용자의 명령(i,d,p,b,q)에 따라 해당 작업을 수행하기 위해 switch사용
        case 'i': case 'I':           
            data = getElement();
            enQueue(cQ, data);
            break;
        case 'd': case 'D':
            deQueue(cQ);
            break;
        case 'p': case 'P':
            printQ(cQ);
            break;
        case 'b': case 'B':
            debugQ(cQ);
            break;
        case 'q': case 'Q':
               freeQueue(cQ);
            break;
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }

    }while(command != 'q' && command != 'Q');  // 사용자가 'q' 또는 'Q'를 입력할 때까지 반복하기 위함.

    return 1;
}

QueueType *createQueue()
{
    QueueType *cQ;
    cQ = (QueueType *)malloc(sizeof(QueueType));  // 큐를 위한 메모리를 동적으로 할당
    cQ->front = 0;  // front를 0으로 초기화
    cQ->rear = 0;  // rear를 0으로 초기화
    return cQ;  // 생성된 큐 반환
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1;  // 큐가 NULL인 경우, 이미 해제된 것으로 간주 -> 1 반환
    free(cQ);  // 큐에 동적으로 할당된 메모리를 해제
    return 1;  // 메모리 해제가 성공적으로 이루어졌음을 나타냄.
}

element getElement()
{
    element item;
    printf("Input element = ");
    scanf(" %c", &item);  // 사용자로부터 요소를 입력받아 item에 넣음.
    return item;  // 입력받은 요소를 반환
}

int isEmpty(QueueType *cQ)
{
    if(cQ->front == cQ->rear)        //구조체 포인터를 통해 멤버에 접근하기 위해 ->를 사용함.
        return 1;  // front와 rear가 같은 위치를 가리키면 큐는 비어있는 상태를 나타냄
    return 0;  // 그렇지 않으면 큐에 요소가 하나 이상 들어있음을 나타냄.
}

int isFull(QueueType *cQ)
{
    if(cQ-> front == (cQ->rear + 1) % MAX_QUEUE_SIZE) {  //isEmpty와 isFull을 구분하기 위해 공간 하나를 비워두었기 때문.
        return 1;  // front가 rear 다음 위치를 가리키면 큐는 가득 찼음을 나타냄.
    }
    return 0;  // 그렇지 않으면 큐에 공간이 하나 이상 남았음을 나타냄.
}

void enQueue(QueueType *cQ, element item)
{
    if(isFull(cQ)) {
        printf("Queue is full!! please delete\n");  // 큐가 가득 찬 경우에는 가득 찼다는 메세지를 출력하고 함수를 종료
        return;
    }	
    else {
        cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;  // rear를 한 칸 뒤로 이동
        cQ->queue[cQ->rear] = item;  // 요소 삽입
    }
}

void deQueue(QueueType *cQ)
{
    if(isEmpty(cQ)) {
        printf("Queue is empty !! please input item");  // 큐가 비어 있는 경우에는 비어있다는 메시지를 출력하고 함수를 종료
        return;
    }
    else {
        cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE;  // front를 한 칸 앞으로 이동        
    }
}

void printQ(QueueType *cQ)
{
    int i, first, last;

    first = (cQ->front + 1)%MAX_QUEUE_SIZE;  // 출력을 시작할 위치를 결정
    last = (cQ->rear + 1)%MAX_QUEUE_SIZE;  // 출력을 종료할 위치를 결정

    printf("Circular Queue : [");

    i = first;
    while(i != last){  // first부터 last까지의 요소 출력
        printf("%3c", cQ->queue[i]);
        i = (i+1)%MAX_QUEUE_SIZE;
    }
    printf(" ]\n");
}

void debugQ(QueueType *cQ)
{
    printf("\n---DEBUG\n");
    for(int i = 0; i < MAX_QUEUE_SIZE; i++)
    {
        if(i == cQ->front) {
            printf("  [%d] = front\n", i);  // front 위치 출력
            continue;
        }
        printf("  [%d] = %c\n", i, cQ->queue[i]);  // 각 위치의 요소 출력
    }
    printf("front = %d, rear = %d\n", cQ->front, cQ->rear);  // front와 rear의 위치 출력
}