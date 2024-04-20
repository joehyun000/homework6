/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum {
    lparen = 0,  /* ( 왼쪽 괄호 */
    rparen = 9,  /* ) 오른쪽 괄호*/
    times = 7,   /* * 곱셈 */
    divide = 6,  /* / 나눗셈 */
    plus = 5,    /* + 덧셈 */
    minus = 4,   /* - 뺄셈 */
    operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];       /* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];     /* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];        /* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];            /* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;     /* evalStack용 top */

int evalResult = 0;        /* 계산 결과를 저장 */

void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char *c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main() {
    char command;

    do {
        printf("[----- [박조현] [2021053017] -----]\n");
        printf("----------------------------------------------------------------\n");
        printf("               Infix to Postfix, then Evaluation               \n");
        printf("----------------------------------------------------------------\n");
        printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command);

        switch(command) {
        case 'i': case 'I':
            getInfix();  // 사용자로부터 중위 표현식을 입력받는 case
            break;
        case 'p': case 'P':
            toPostfix();  // 중위 표현식을 후위 표현식으로 변환하는 case
            break;
        case 'e': case 'E':
            evaluation();  // 후위 표현식을 계산하는 case
            break;
        case 'd': case 'D':
            debug();  // 프로그램의 상태를 디버깅하는 case
            break;
        case 'r': case 'R':
            reset();  // 프로그램을 초기화하는 case
            break;
        case 'q': case 'Q':
            break;  // 프로그램을 종료하는 case
        default:
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;  // 잘못된 명령을 입력 시 , 경고 메시지를 출력
        }

    } while(command != 'q' && command != 'Q');

    return 0;
}

void postfixPush(char x) {
    postfixStack[++postfixStackTop] = x;  // postfixStack에 x를 push , 코드 수를 단순화 하기 위해 후위 대입 연산자를 사용
}

char postfixPop() {
    char x;
    if (postfixStackTop == -1)
        return '\0';  // postfixStack이 비어 있을 시, NULL 문자를 반환
    else {
        x = postfixStack[postfixStackTop--];  // postfixStack에서 top 요소를 pop, 코드 수를 단순화 하기 위해 전위 대입 연산자를 사용
    }
    return x;
}

void evalPush(int x) {
    evalStack[++evalStackTop] = x;  // evalStack에 x를 push, 코드 수를 단순화 하기 위해 후위 대입 연산자를 사용
}

int evalPop() {
    if (evalStackTop == -1)
        return -1;  // evalStack이 비어 있으면, -1을 반환
    else
        return evalStack[evalStackTop--];  // evalStack에서 top 요소를 pop, 코드 수를 단순화 하기 위해 전위 대입 연산자를 사용
}

void getInfix() {
    printf("Type the expression >>> ");
    scanf("%s", infixExp);  // 사용자로부터 중위 표현식을 입력받음
}

precedence getToken(char symbol) {    // 각 기호에 대한 우선순위 반환
    switch(symbol) {
    case '(' : return lparen;
    case ')' : return rparen;
    case '+' : return plus;
    case '-' : return minus;
    case '/' : return divide;
    case '*' : return times;
    default : return operand;  
    }
}

precedence getPriority(char x) {
    return getToken(x);  // x의 우선순위 반환
}

void charCat(char* c) {    // postfixExp 문자열에 문자를 추가하는 함수
    if (postfixExp == '\0')          // 만약 postfixExp가 비어 있을 시, c의 첫 번째 문자를 복사
        strncpy(postfixExp, c, 1);
    else                             // 그렇지 않을 시, c의 첫 번째 문자를 postfixExp의 끝에 추가
        strncat(postfixExp, c, 1);
}

void toPostfix() {
    /* infixExp의 문자 하나씩을 읽기위한 포인터 */
    char *exp = infixExp;
    char x; /* 문자하나를 임시로 저장하기 위한 변수 */

    /* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
    while (*exp != '\0') {
        if (getToken(*exp) == operand) {
            charCat(exp);  // 피연산자: 바로 postfixExp에 추가
        }
        else if (getToken(*exp) == lparen) {
            postfixPush(*exp);  // 왼쪽 괄호: 스택에 push
        }
        else if (getToken(*exp) == rparen) {
            while (postfixStack[postfixStackTop] != '(') {
                charCat(&postfixStack[postfixStackTop]);  // 오른쪽 괄호: 왼쪽 괄호가 나올 때까지 스택에서 pop하여 postfixExp에 추가
                postfixPop();
            }
            postfixPop();  // 왼쪽 괄호 pop
        }
        else {
            while (postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) {
                charCat(&postfixStack[postfixStackTop]);  // 스택의 top 연산자 우선순위 >= 현재 연산자보다 우선순위이면 
                postfixPop();                            //pop하여 postfixExp에 추가
            }
            postfixPush(*exp);  // 현재 연산자를 스택에 push
        }
        exp++;
    }
    /* 스택에 남아있는 연산자들을 모두 후위 표기법 문자열에 추가 */
    while (postfixStackTop != -1) {
        charCat(&postfixStack[postfixStackTop]);  // 스택에 남아있는 모든 연산자를 pop하여 postfixExp에 추가, 그래야 변환이 잘 마무리됨
        postfixPop();
    }
}

void debug() {
    printf("\n---DEBUG\n");
    printf("infixExp =  %s\n", infixExp);
    printf("postExp =  %s\n", postfixExp);
    printf("eval result = %d\n", evalResult);

    printf("postfixStack : ");
    for(int i = 0; i < MAX_STACK_SIZE; i++)
        printf("%c  ", postfixStack[i]);  // 프로그램의 상태 전체를 출력

    printf("\n");
}

void reset() {        // 모두 0으로 초기화 
    infixExp[0] = '\0';
    postfixExp[0] = '\0';
    for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}


void evaluation() {
    char *exp = postfixExp; // 후위 표기법으로 표현된 수식을 가리키는 포인터

    // 스택 초기화
    evalStackTop = -1;

    while (*exp != '\0') {
        if (getToken(*exp) == operand) {
            // 피연산자인 경우 스택에 푸시
            evalPush(*exp - '0'); // 문자를 숫자로 변환하여 푸시
        } else {
            // 연산자인 경우 스택에서 피연산자를 팝하여 계산하고 결과를 다시 스택에 푸시
            int operand2 = evalPop();
            int operand1 = evalPop();
            int result;

            // 연산 수행
            switch (*exp) {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    result = operand1 / operand2;
                    break;
                default:
                    printf("Invalid operator\n");
                    return; // 잘못된 연산자인 경우 함수 종료
            }

            // 연산 결과를 스택에 푸시
            evalPush(result);
        }
        exp++;
    }

    // 최종 결과는 스택의 맨 위에 남아있는 값
    evalResult = evalPop();
    printf("Evaluation result: %d\n", evalResult);
}