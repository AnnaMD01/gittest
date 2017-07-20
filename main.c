#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#define _XOPEN_SOURCE 600

pthread_barrier_t barrier;
pthread_mutex_t mutex;
struct list{
	int val;
	struct list * next;
	void (*pf)(int);
};
struct list *head = NULL;


void print_node(int val)
{   
	printf("%d ",val);
}


// add a node at the end of the list
void add_node(int data)
{  
   printf("ADD %ld\n",pthread_self() );
   struct list* node =(struct list*) malloc(sizeof(struct list));
   node->val = data;
   node->next = NULL;
   node->pf = &print_node;

	pthread_mutex_lock(&mutex);   	
   // if the list is empty
   if(head == NULL)  
   {  
           head = node;
   } 
   else {  
	       struct list* temp = head; 
           while(temp->next != NULL) 
		   {
			       temp=temp->next; 
		   } 
		   temp->next=node;
        } 
    pthread_mutex_unlock(&mutex);
} 
         
               
void print_list()
{    
    printf("PRINT LIST %ld\n",pthread_self() );
	
	pthread_mutex_lock(&mutex);
	struct list* temp = head;
	if(head!=NULL) {
	        while(temp != NULL) { 
			          //printf("%d ", temp->val); 
                      temp->pf(temp->val); 
					  temp  = temp->next; 
					            } 
	        printf("\n");   
			      }
	else 
	        printf("Nothing to show. The list is empty!\n");
	pthread_mutex_unlock(&mutex);			            
}


void flush_list()
{     
    printf("FLUSH LIST %ld\n",pthread_self() );
    
	pthread_mutex_lock(&mutex);
    while(head!=NULL) { 
	        struct list *temp;
	        temp=head;
	        head=head->next;
	        free(temp);
	                  }
					    
	pthread_mutex_unlock(&mutex);
}
	
	
void delete_node(int valoare)
{  
    printf("DELETE NODE %ld\n",pthread_self() );
	
	pthread_mutex_lock(&mutex);
    if(head == NULL) 
	         printf("List is empty, we have nothing to erase!");
	else 
	         if( head->next == NULL && head->val == valoare) 
			         free(head);  
	         else {   
	                 if(head->val == valoare) { 
					          struct list* p = head;
			                  head = head->next; free(p); 
}
	struct list* predecesor = head;
    struct list* curent = predecesor->next; 
	         
	while ( curent->next!=NULL && curent->val!=valoare) { 
	         predecesor=curent;
	         curent=curent->next; 
}
	if(curent->val == valoare) {  
	         predecesor->next = curent->next;
			 free(curent);
}   
	else 
	         printf("this specific element is not in this list!\n");					                                                 
	         	                                                            
}
	pthread_mutex_unlock(&mutex);	
}


void sort_list()
{  

   printf("SORT LIST %ld\n",pthread_self() );
	
	pthread_mutex_lock(&mutex);
   struct list *p= head;
   struct list *c= p->next;
   
   int swap = 1;  
   while (swap!=0){
          swap=0;
          p= head;
          c= p->next;
          while( p->next!=NULL  ) { 
  	             if( p->val > c->val ) {      
				     int temp = p->val; 
  	                 p->val = c->val;
  	                 c->val = temp;
					 swap=1; 
} 
  	      p=c;
	      c=c->next;								 
} 
}
	pthread_mutex_unlock(&mutex);
}


void* sinc (void* arg)
{ 
	int *p = (int*)arg;
	int i = *p;	

    // thread-urile asteapta la bariera
    int ultim = pthread_barrier_wait(&barrier);
	if(ultim == PTHREAD_BARRIER_SERIAL_THREAD) 
	{ 
	    printf("This is the last thread!\n");
	} 
	
 	    switch(i){
			case 0:
	    		add_node(2);
        		add_node(4);
        		add_node(10);
        		delete_node(2);
        		sort_list();
        		delete_node(10);
        		delete_node(5);
            	break;
        	case 1:
		    	add_node(11);
            	add_node(1);
            	delete_node(11);
            	add_node(8);
            	print_list();
				break;
   	    	case 2:
				add_node(30);
                add_node(25);
                add_node(100);
                sort_list();
                print_list();
                delete_node(100);
                break;
            default:
				printf("Invalid number\n");    
                 }
}

int main(int argc, char *argv[], char** environ) {
	
	
   //WORKS WELL
   //add_first(7);
   //add_first(2);
   //add_first(5);
   //add_first(1);
   //add_first(8);
   //print();
   //delete_node(5);
   //print(); 
   //sort_list();
   //print();
   //delete();




   // WORKS WELL
   //add_node(8);
   //add_node(1);
   //add_node(5);
   //add_node(2);
   //add_node(7);
   //print_list();
   //delete_node(5);
   //print_list();
   //sort_list();
   //print_list();
   //flush_list();
   //print_list();

   printf("The id of the main thread is %ld\n",pthread_self());

   pthread_t threads[3];
   int i;
   pthread_barrier_init(&barrier, NULL, 3);
   
   int v[] = {0,1,2};
   for(i=0;i<3;i++)
{
	   pthread_create(&threads[i], NULL, sinc, &v[i] );
}

  
   for(i=0;i<3;i++)
{ 
      pthread_join(threads[i], NULL);
}

pthread_barrier_destroy(&barrier);



   print_list();
   flush_list();
   //printf("The end of the program");

 
	return 0;
} 

