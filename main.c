#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#define _XOPEN_SOURCE 600


pthread_barrier_t barrier;

struct list{
	int val;
	struct list * next;
};
struct list *head = NULL;


void add_first(int data){
	 struct list *nod =(struct list*) malloc(sizeof(struct list));
     nod->val=data;
	 nod->next=head; 
	 head=nod;
}

// add a node at the end of the list
void add_node(int data)
{  
   printf("ADD %d\n",pthread_self() );
   struct list* node =(struct list*) malloc(sizeof(struct list));
   node->val = data;
   node->next = NULL;

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
} 
         
               
void print_list()
{    
    printf("PRINT LIST %d\n",pthread_self() );

	struct list* temp = head;
	if(head!=NULL) {
	        while(temp != NULL) { 
			          //printf("%d ", temp->val);    
			          print(temp);
	                  temp  = temp->next; 
					            } 
	        printf("\n");   
			      }
	else 
	        printf("Nothing to show. The list is empty!\n");			            
}

void print (struct list* node)
{
	printf("%d ", node->val);	
	
}

void flush_list()
{     
    printf("FLUSH LIST %d\n",pthread_self() );

    while(head!=NULL) { 
	        struct list *temp;
	        temp=head;
	        head=head->next;
	        free(temp);
	                  }  
}
	
	
void delete_node(int valoare)
{  
    printf("DELETE NODE %d\n",pthread_self() );

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
}


void sort_list()
{  

   printf("SORT LIST %d\n",pthread_self() );

   struct list *p= head;
   struct list *c= p->next;
   
   int swap = 1;  
   while (swap!=0){
          swap=0;
          struct list *p= head;
          struct list *c= p->next;
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
}


void* sinc (void* arg)
{ 
	int i = (int)arg;
   //	printf("\nValoarea lui 1 este %d\n ", i);	

    // thread-urile asteapta la bariera
    int ultim = pthread_barrier_wait(&barrier);
	if(ultim == PTHREAD_BARRIER_SERIAL_THREAD) 
	{ 
	    printf("This is the last thread!\n");
	} 
	
 	if(i==0) {    
	    add_node(2);
        add_node(4);
        add_node(10);
        delete_node(2);
        sort_list();
        delete_node(10);
        delete_node(5);
}
   	else 
	    if(i==1) {      
		    add_node(11);
            add_node(1);
            delete_node(11);
            add_node(8);
            print_list();
}
   	    else 
		    { 
			    add_node(30);
                add_node(25);
                add_node(100);
                sort_list();
                print_list();
                delete_node(100);
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
   //add_last(8);
   //add_last(1);
   //add_last(5);
   //add_last(2);
   //add_last(7);
   //print();
   //delete_node(5);
   //print();
   //sort_list();
   //print();
   //delete();
   //print();

   printf("The id of the main thread is %d\n",pthread_self());

   pthread_t threads[3];
   int i;
   pthread_barrier_init(&barrier, NULL, 3);
   
   for(i=0;i<3;i++)
{
	   pthread_create(&threads[i], NULL, sinc,(void*)i );
}

  
   for(i=0;i<3;i++)
{ 
      pthread_join(threads[i], NULL);
}

pthread_barrier_destroy(&barrier);



   print_list();
   flush_list();

	return 0;
}
