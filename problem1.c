#include<stdio.h>
// trebuie inclusa si o biblioteca pentru fire de executie

struct list
{
   int val;  // sau char* val se putea(generalizare)
   list* next;
};
list* head = NULL;

//add a new node at the end of the list
void add_node(list *head, int valoare) // vad transmiterea
{
  list* node = malloc(sizeof(list));
  node->val = valoare;
  node->next = NULL;

  if(head==NULL) // if the list is empty 
  	{  head = node; }
  else {  list* temp = head;
  	      while(temp->next!=NULL) tem=temp->next;
     temp->next=node;
     free(temp);
       }
}

void flush_list(list *head)  //vad transmiterea
{
  
  list* node = head;
  while( node->next!=NULL) { 
  	            list* temp;
                temp = node->next;
                node = node->next;
                free(temp);  }
         head =NULL;      
}


void print_list(list *head) // vad transmiterea
{  
	
	while( head != NULL )
	{  printf("%d ", head->val);
       head = head->next;    

	}

}


// delete the node
// as putea sa-i dau sa intoarca pozitia elementului sters (daca exista)
// sau -1 in cazul in care nu exista in lista
//face stergerea dupa valoare
void delete_node(list *head, int valoare)
{  
     // daca lista e egoala
	if(head == NULL) printf("List is empty, we have nothing to erase!");
	//daca lista are doar un element 
	else if(  ) {  free(head->next);
	                                     free(head);  }
	// daca lista are mai multe elemente, 2 >                                  
	         else {   list* predecesor = head;
	         	      list* curent = predecesor->next;
		while ( curent->next!= NULL && curent->val!=valoare) { predecesor = predecesor->next;
		                                                       curent = curent->next; }
        if(curent->val==valoare) { precedent=curent->next;
                                   free(curent);  }
        if(curent->next==NULL && curent->val==valoare){  predecesor->next=NULL;
                                                         free(curent);  }
                   }
}


int main()
{
   

	return 0;
}
