#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define NUM_THREADS 5
#define MAX_NUM 10000

//変数宣言
void *do_calc(void *);
pthread_t tid[NUM_THREADS];
pthread_mutex_t mutex;
int param[NUM_THREADS];
int array[MAX_NUM];
unsigned char check[MAX_NUM +1];
int a;

int main(){
    //lock/unlockの処理用
    pthread_mutex_init(&mutex,NULL);
    //配列にMAX_NUM分の数字を格納
    for (int i = 0; i < MAX_NUM; i++)
    {
        array[i] = i;
    }
    //スレッドの立ち上げ
    for (int i = 0; i < NUM_THREADS; i++)
    {
        param[i] = i;
        pthread_create(&tid[i],NULL,do_calc,&param[i]);
    }
    //スレッドの終了
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tid[i],NULL);
    }
    //出力
    for (int i = 0; i < MAX_NUM; i++)
    {
        if(check[i] == 1){
        printf("check = %d\n",i);
        }
    }
    printf("all thread terminated\n");
    pthread_mutex_destroy(&mutex);
    return 0;    
}

void *do_calc(void *arg){
    //変数宣言
    int inum,istart,iend,*id;
    id = arg;

    //スレッドを分割
    inum = MAX_NUM/NUM_THREADS;
    istart = *id * inum;
    iend = istart + inum;

    for (int i = istart; i < iend; i++)
    {
        pthread_mutex_lock(&mutex);
        //素数を見つける箇所
        for (int j = 2; j < array[i-1]; j++)
        {
            if(array[i]%j == 0){
                a++;
            }
        }
        //一度も割れていなければcheck[i]=1    
        if(a == 0){
            check[i] = 1;
        }else{
            check[i] = 0;
            a = 0;
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}