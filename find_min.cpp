#include<iostream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<pthread.h>
#include<unistd.h>
#include<limits.h>
#include<math.h>
#define MAX pow(10,7)
#define PERThread 100000

using namespace std;

pthread_mutex_t MUTEX;
pthread_barrier_t BARRIER;

class range{
public:   int high;
          int low;
          vector<int> arr;
          int *finalMin;

          range(){}

          range(int l,int h, vector<int>& a,int* final)
          {
            low=l;
            arr=a;
            if(arr.size()<h)
                high=arr.size();
            else
                high=h;

             finalMin=final;
          //cout<<"value passed is "<<final<<" and asserted value = "<<finalMin<<endl;

          }
};


void * findMin(void *data)
{
 range *item=(range*)  data;
 int min=item->arr[item->low];

 for(int i=item->low;i<item->high;i++){
   if(item->arr[i]<min)
         min=item->arr[i];
  //if(item->arr[i]==37)    cout<<"37 found"<<endl;
 }

    //we have the min, all we need to do is update;

    //cout<<"finalMin element is "<<*(item->finalMin)<<endl;
    pthread_mutex_lock(&MUTEX);

    if(min < *(item->finalMin))
        *(item->finalMin)=min;

    //cout<<"min element is "<<min<<endl;
    pthread_mutex_unlock(&MUTEX);

    pthread_barrier_wait(&BARRIER);


    //cout<<"finalMin element is "<<*(item->finalMin)<<endl;
}

int ceil_div(int a,int b){
int pro=a/b;

if(pro*b<a)  pro++;

return pro;
}

int main()
{
vector<int> arr(MAX);
int finalMin=INT_MAX;
pthread_mutex_init(&MUTEX, NULL);


for(int i=0;i<arr.size();i++)
  {
     cin>>arr[i];
    //if(arr[i]==37)   cout<<"PARTY"<<endl;
  }


int no_ofThreads= ceil_div(arr.size(),PERThread);

pthread_t threads[no_ofThreads];

pthread_barrier_init(&BARRIER,NULL,no_ofThreads);

for(int i=0;i<no_ofThreads;i++)
{
    //cout<<i*5<<"to"<<;
    range *data =new range(i*PERThread,(i+1)*PERThread-1,arr,&finalMin);
    pthread_create(&threads[i], NULL, findMin,data);

    //pthread_join(threads[i],NULL);
}

pthread_barrier_wait(&BARRIER);
pthread_barrier_destroy(&BARRIER);

//usleep(10000000);
cout<<"The number of threads generated is "<<no_ofThreads<<endl;
cout<<"Final minimum element is "<<finalMin<<endl;
  return 0;
}
