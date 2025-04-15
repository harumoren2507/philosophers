#include <pthread.h>
//#include "tlpi_hder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void *threadFunc(void *arg)
{
    char* s = (char *)arg;
    int *result = malloc(sizeof(int));
    *result = 42;
    printf("%s", s);
    pthread_exit((void *)result);
}

int main(void)
{
    pthread_t tl;
    void    *thread_result;
    int s;

    s = pthread_create(&tl, NULL, threadFunc, "Hello, World from thread\n\n");
    //if (s != 0)
        //errExitEN(s, "pthread_create");
    printf("Message from main()\n\n");
    s = pthread_join(tl, &thread_result);
    //if (s != 0)
        //errExitEN(s, "pthread_join");
    printf("thread returned %d\n", *(int *)thread_result);
    exit(EXIT_SUCCESS);
}

/*

   pthread_createの仕様
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                void *(*start_routine) (void *), void *arg);
引数の説明

pthread_t *thread:

新しく作成されるスレッドのID（識別子）を格納するためのポインタ
スレッド作成に成功すると、この変数にスレッドIDが設定されます


const pthread_attr_t *attr:

スレッドの属性を指定するための構造体へのポインタ
NULLを指定すると、デフォルトの属性でスレッドが作成されます
スタックサイズ、スケジューリングポリシー、優先度などを設定可能


void *(*start_routine) (void *):

新しいスレッドで実行される関数へのポインタ
この関数はvoid*型の引数を1つ受け取り、void*型の値を返す必要があります


void *arg:

start_routine関数に渡される引数
任意のデータを渡すためのポインタ


phthread_join()の仕様

スレッドの終了を待つ　

*/

