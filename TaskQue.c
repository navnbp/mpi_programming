 #include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


int     threads, n, range,tasks,linked_list_size=0,task_count;
pthread_mutex_t   mutex;
pthread_rwlock_t  rwlock;

struct linked_list
{
    int data;
    struct linked_list *next;
};

 struct linked_list *head,*tail,*new,*temp;


void insert(int value)
{

    if (tail == NULL)
    {
        tail = (struct linked_list *)malloc(sizeof(struct linked_list));
        tail->next = NULL;
        tail->data = value;
        head = tail;
    }
    else
    {
        new=(struct linked_list *)malloc(sizeof(struct linked_list));
        tail->next = new;
        new->data = value;
        new->next = NULL;
 
        tail = new;
    }
    linked_list_size++;
    printf("Inserted %d \n",value );
 
}
 


 
void  display()
{

    temp=head;
    if(temp==NULL)
    {
    return;
    }
     printf("Elements are :");
    while(temp!=NULL)
    {
      printf("%d ",temp->data);
      temp=temp->next;
    }
    printf("\n");
  
}




void member(int value)
{
    int  check=0;
    temp = head;

   if ((temp == NULL) && (tail == NULL))
    {
        printf("List is empty");
        return;
    }

    while (temp != tail)
    {
        if(temp->data==value)
        {
            check=1;
              printf("%d is in the list ", temp->data);
              break;
          }
        temp = temp->next;
    }

    if (temp == tail)
        if(temp->data==value)
        {
              printf("%d is in the list ", temp->data);
              check=1;
        }


    if(check==0)
 printf("%d is not in the linked list ", value);
printf("\n");
}


void delete()
{
    temp = head;
 
    if (temp == NULL)
    {
        printf("List is empty");
        return;
    }
    else
        if (temp->next != NULL)
        {
            temp = temp->next;
            printf(" Removed  %d from the linked list", head->data);
            free(head);
            head = temp;
        }
        else
        {
            printf(" Removed  %d from the linked list", head->data);
            free(head);
            head = NULL;
            tail = NULL;
        }
        linked_list_size--;
           printf("\n");


}
 


  void *Task_Queue(void* rank) {

    int i,thread_range,ct;
   long local_rank = (long) rank;
  
   thread_range = task_count/threads; 
 pthread_mutex_lock(&mutex);

   for (i = 0; i < thread_range; i++) {
   ct=i%3;

   switch(ct)
   {
    case 0 :  
            pthread_rwlock_wrlock(&rwlock);
            insert(linked_list_size+1);
            pthread_rwlock_unlock(&rwlock);
            
            pthread_rwlock_rdlock(&rwlock);
            display();
            pthread_rwlock_unlock(&rwlock);
             printf("\n");
            break;
    case 1: 
            pthread_rwlock_wrlock(&rwlock);
            delete();
            pthread_rwlock_unlock(&rwlock);
             printf("\n");
            break;
    case 2 : 
            pthread_rwlock_rdlock(&rwlock);
            member(rand()%17);
            pthread_rwlock_unlock(&rwlock);
             printf("\n");
            break;

     default: break;
   }
 
    


   }
      pthread_mutex_unlock(&mutex);


 

    return NULL;
}  



 

int main()
{
    int no, ch, e;
 
    long        i;
    pthread_t*  thread_handles;  


    head =  NULL;
    tail =NULL;

     printf("Enter the number of threads\n");
     scanf("%d",&threads);

     printf("Enter the number of tasks to be performed\n");
     scanf("%d",&task_count);

     for(int j=1;j<10;j++)
     {
          insert(j);
     }
printf("----------------------\n");

     thread_handles = malloc (threads*sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);
 pthread_rwlock_init(&rwlock, NULL);
 
 for (i = 0; i < threads; i++) 
         pthread_create(&thread_handles[i], NULL, Task_Queue,  (void*) i);
    

    for (i = 0; i < threads; i++) 
        pthread_join(thread_handles[i], NULL);

  pthread_mutex_destroy(&mutex);
    free(thread_handles);

return 0;
}



 
