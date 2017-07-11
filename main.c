#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include<string.h>
#include<signal.h>
#include<stdbool.h>
#include<pthread.h>
#define show_var(a) { printf("Variable %s has value %d\n", #a, a); }
//  #include<windows.h>  -- scriere citire fisiere
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

 struct list{
	int val;
	struct list * next;
};
 struct list *head = NULL;


void add_first(int data)
{
	 struct list *nod =(struct list*) malloc(sizeof(struct list));
     nod->val=data;
	
	nod->next=head; // nod->next=NULL;
	head=nod;
}

void add_last(int data)
{
  struct list* node =(struct list*) malloc(sizeof(struct list));
  node->val = data;
  node->next = NULL;

 
  if(head == NULL) // if the list is empty 
  	{  head = node;
	  }
  else {   struct list* temp = head;

            while(temp->next != NULL) {
			temp=temp->next; } 
		   if(temp->next==NULL ) temp->next=node;
      free(temp); } 
         }

void print()
{
	struct list* temp = head;
	while(temp != NULL) {  printf("%d ",temp->val);
	                           temp  = temp->next; } 
	              printf("\n");             
}


void delete()
{
	struct list *temp;
	while(head!=NULL) {  temp=head;
	 head=head->next;
	 free(temp);
	}  head = NULL;
}
	
void delete_node(int valoare)
{  // printf("\nIn function\n");
	 // daca lista e goala
	if(head == NULL) printf("List is empty, we have nothing to erase!");
	// daca lista are doar un element
	else if( head->next == NULL && head->val == valoare) {  free(head);  }
	// daca lista are mai multe elemente, 2 >                                  
	         else {   
	         if(head->val == valoare) { struct list* p = head;
			 head = head->next; free(p);  }
	         struct list* predecesor = head;
	         struct list* curent = predecesor->next; //curent e al doilea element in lista
	         
	         	      while ( curent->next!=NULL && curent->val!=valoare) { predecesor=curent;
	         	                                                            curent=curent->next; }
	         	           if(curent->val == valoare) {  predecesor->next = curent->next;
						                                 free(curent);	}   
									else printf("this specific element is not in this list!\n");					                                                 
	         	                                                            
}	
}

// lower to the higher value of the list
//schimbarea referintelor
void sort_list()
{ 
  /*struct list * p = head; //aici e pb
  struct list * c = p->next; 
  struct list * u = c->next;
  int swap = 1; // un fel de variabila booleana
 while (swap!=0){
  swap=0;
  
  while( c->next!=NULL  ) // daca nu am doar un singur nod
  { 
  	if( c->val >   u->val ) {      c->next = u->next;
  	                               u->next = c; 
  	                               p = u;
								   swap=1; } 
   p=c; 
   c=u;
   u=u->next;  									 
  	
  }  }  */
  
  // aici ruleaza dar nu ordoneaza corect, cred ca e de la swap.
  struct list *p= head;
  struct list *c= p->next;
   
  int swap = 1;  // un fel de variabila booleana
  while (swap!=0){
  swap=0;
  
  while( p->next!=NULL  ) // daca nu am doar un singur nod
  { 
  	if( p->val >   c->val ) {      int temp = p->val;
  	                               p->val = c->val;
  	                               c->val = temp;
								   swap=1; } 
  	p=c;
	c=c->next;								 
   }  }
}

int main(int argc, char *argv[], char** environ) {
	
	

add_first(7);
add_first(2);
add_first(5);
add_first(1);
add_first(8);
print();
//printf("%d\n",head->val);
//printf("%d\n",head->next->val);
//delete_node(5);
//print(); 
sort_list();
print();
delete();


// o pb pe alocarea add_last

//if(head==NULL) printf("ok\n");
//add_last(1);
//add_last(2);
//add_last(3);
//add_last(4);
//add_last(5);
//printf("main after add_last\n");
//printf("%d %d\n",head->val,head->next->val);
//print();
//printf("main after print method\n");
//delete();
//printf("main after delete method\n");
// infinte loop

















	return 0;
}
