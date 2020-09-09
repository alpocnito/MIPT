#include "../Headers/color.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define Assert(expr) if ((expr) == 0) {                   \
                                                          \
   PRINT_RED(BOLD(#expr "   is not true\n"));             \
   printf("In %s:%d\n", __PRETTY_FUNCTION__, __LINE__);   \
   return 0;                                              \
} 

#define LIST_SYMBOL "s"
typedef char* Elem_t;

//////////////////////////////// LIST //////////////////////////////
struct ListNode {
  
  Elem_t data;
  ListNode* next;
};

struct List {
  
  ListNode* first_elem;
  size_t number_nodes;
  ListNode* last_elem;
};

ListNode memory[33927810];

////////////////////////////////////////////////////////////////////

int ListValid  (List* list);
void PrintList (List* list);

void ListInit    (List* list);
void ListDisinit (List* list);

extern int ListPush   (List* list, Elem_t value);
int ListDelete (List* list, int number_node);

int ListValid(List* list) {
  return 1;
}


void ListInit(List* list) {
  assert(list != NULL);

  list->first_elem = NULL;
  list->last_elem  = NULL;
  list->number_nodes = 0;
}

void ListDisinit(List* list) {
  //list = 0;
  assert(ListValid(list));
  
  ListNode* next_elem = list->first_elem;
  ListNode* prev_elem = NULL;
  for (int i = 0; i < list->number_nodes; ++i) {
    
    prev_elem = next_elem;
    next_elem = next_elem->next;
//    free(prev_elem);
  }
}

int LListPush(List* list, Elem_t value) {
  static int number_calls = 0;
  //printf("%d\n", ++number_calls);  
  //ListNode* new_node = (ListNode*)calloc(1, sizeof(ListNode));
  ListNode* new_node = memory + number_calls++;
 
  //init node
  new_node->data = value;
  new_node->next = NULL;
  
  if (list->number_nodes == 0) {
    list->first_elem = list->last_elem = new_node;
  }
  else {
    list->last_elem->next = new_node;
    list->last_elem = new_node;
  }
  list->number_nodes += 1;
  
  return 0;
}

int ListDelete(List* list, int number_node) {
  assert(ListValid(list));
  
  if (number_node >= list->number_nodes)
    return 1;
  
  //if we delete first element
  if (number_node == 0) {
    
    ListNode* new_first_elem = list->first_elem->next;
    //free(list->first_elem);
    list->first_elem = new_first_elem;

    list->number_nodes -= 1;
    return 0;
  }

  ListNode* prev    = list->first_elem;
  ListNode* current = list->first_elem->next;
  for (int i = 1; i < number_node; ++i) {
    
    current = current->next;
    prev    = prev->next;
  }
  
  ListNode* new_current = current->next;
  //free(current);
  prev->next = new_current;

  list->number_nodes -= 1;
  
  //if we delete last element
  if (new_current == NULL)
    list->last_elem = prev;
  
  return 0;
}

void PrintList(List* list) {
  assert(list != NULL);

  for (ListNode* it_node = list->first_elem; it_node != NULL; it_node = it_node->next)
    printf("%" LIST_SYMBOL ",", it_node->data);
  printf("\n");

  //PRINT_MAGENTA(N("first_elem   = ")); printf("%" LIST_SYMBOL "\n", list->first_elem->data);
  //PRINT_MAGENTA(N("number_nodes = ")); printf("%lu\n", list->number_nodes);
  //PRINT_MAGENTA(N("last_elem    = ")); printf("%" LIST_SYMBOL "\n", list->last_elem->data);
}

