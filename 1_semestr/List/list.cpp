#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../Headers/color.h"

#define LIST_SYMBOL "d"
#define Assert(expr) if ((expr) == 0) {                   \
                                                          \
   PRINT_RED(BOLD(#expr "   is not true\n"));             \
   printf("In %s:%d\n", __PRETTY_FUNCTION__, __LINE__);   \
   return 0;                                              \
}                                                         

typedef char* Elem_t;

const size_t INIT_LIST_SIZE = 10;
const double EXPAND_COEF    = 2;  // to ExpandList
const int POISON_VALUE      = 0;  
const int FREE_VALUE        = -1; // if value is free, prev of this value == FREE_VALUE

struct List {

   Elem_t* data;     //!< Array with our data
   int* next;        // Array with pointers on the next element
   int* prev;        // Array with pointers on the previous element
   
   int head;         // Physical number of the List's first element
   int tail;         // Physical number of the List's last element
   int free;         // Physical number of the first free element
   
   size_t size;      // Number of elements in the List
   size_t maxsize;   // Size of the data, next and prev

   bool sorted;      // for SoftPushBack and SoftDelete
};

bool ListInit    (List* list, size_t size = INIT_LIST_SIZE);
void ListDisinit (List* list);

int ListInsert    (List* list, int physical_addr, Elem_t value);
int ListAddAfter  (List* list, int physical_addr, Elem_t value);
int ListPushBack  (List* list, Elem_t value);

int ListDelete (List* list, int physical_addr);

bool ListValid (List* list);
int ExpandList (List* list);
void PrintList (List* list, int maxsize = 3);

int SearchByElem        (List* list, Elem_t value);
int SearchByLogicAddres (List* list, int logic_address);

char* my_realloc(char* old_array, size_t old_size, size_t new_size, size_t one_elem);


bool SortList(List* list);
int ListSoftPushBack(List* list, int physical_addr, Elem_t value);
int ListSoftDelete(List* list, int physical_addr);








char* my_realloc(void* old_array_void, size_t old_size, size_t new_size, size_t one_elem) {
   assert(old_array_void != NULL);
   
   char* old_array = (char*)old_array_void;
   char* new_array = (char*)calloc(new_size * one_elem, sizeof(char));
   
   for (int i = 0; i < one_elem * old_size; ++i)
      new_array[i] = old_array[i];
   
   return new_array;
}

bool ListInit(List* list, size_t size) {
   assert(list != NULL);
   
   list->data = (Elem_t*)calloc(size, sizeof(Elem_t));
   Assert(list->data);

   list->next = (int*)calloc(size, sizeof(int));
   Assert(list->next); 

   list->prev = (int*)calloc(size, sizeof(int));
   Assert(list->prev); 
   
   list->size = 0;
   list->maxsize = size - 1;
   
   list->head = list->tail = POISON_VALUE;

   //initializing list with free values
   for (int i = 1; i < size - 1; ++i)
      list->next[i] = i + 1;
   
   for (int i = 1; i < size; ++i)
      list->prev[i] = FREE_VALUE;
   list->free = 1;
   
   list->sorted = 1;
   return 1;
}

void ListDisinit(List* list) {
   assert(list != NULL);

   free(list->data);
   free(list->next);
   free(list->prev);

   list = NULL;
}

bool ListValid(List* list) {
   assert(list != NULL);

   Assert(list->data != NULL);
   Assert(list->next != NULL);
   Assert(list->prev != NULL);
   
   Assert(list->size <= list->maxsize);
   Assert(list->tail <= list->maxsize);
   Assert(list->head <= list->maxsize);
   Assert(list->free <= list->maxsize);   

   //////////////Go throw list///////////////////////
   if (list->size == 0) return 1;
   
   Assert(list->prev[list->head] == POISON_VALUE);
   Assert(list->next[list->tail] == POISON_VALUE);
   
   if (list->size == 1) return 1;

   if (list->size == 2) {
      Assert(list->next[list->head] == list->tail);
      Assert(list->prev[list->tail] == list->head);
      return 1;
   }

   int counted_size = 2;
   int it = list->next[list->head];
   while (it != list->tail) {

      int prev = list->prev[it];
      int next = list->next[it];
      
      Assert(list->next[prev] == it);
      Assert(list->prev[next] == it);

      it = list->next[it];
      counted_size++;
   }

   Assert(list->next[list->prev[it]] == it);
   ////////////////////////////////////////////////

   Assert(list->size == counted_size);
   return 1;
}

//private!!!!!
int ExpandList(List* list) {
   Assert(ListValid(list));

   int new_size = list->maxsize * EXPAND_COEF;
   Elem_t* new_data = NULL;
   int* new_next = NULL;
   int* new_prev = NULL;

   new_data = (Elem_t*)my_realloc(list->data, list->maxsize + 1, new_size, sizeof(Elem_t));
   Assert(new_data);

   new_next = (int*)my_realloc(list->next, list->maxsize + 1, new_size, sizeof(int));
   Assert(new_next); 

   new_prev = (int*)my_realloc(list->prev, list->maxsize + 1, new_size, sizeof(int));
   Assert(new_prev); 
   
   free(list->data);
   free(list->next);
   free(list->prev);

   list->data = new_data;
   list->next = new_next;
   list->prev = new_prev;
   

   //free values
   if (list->free == POISON_VALUE) {
      
      for (int i = list->maxsize + 1; i < new_size; ++i)
         list->next[i] = i + 1;
      list->next[new_size - 1] = POISON_VALUE;
      list->free = list->maxsize + 1;
   }
   else {

      for (int i = list->maxsize + 1; i < new_size; ++i)
         list->next[i] = i;
      list->free = new_size - 1;
   }
   
   for (int i = list->maxsize + 1; i < new_size; ++i)
      list->prev[i] = FREE_VALUE;
   
   list->maxsize = new_size - 1;

   return 1;
}

int ListAddBefore(List* list, int physical_addr, Elem_t value) {
   assert(ListValid(list));
   assert(physical_addr > 0);
   
   if (list->prev[physical_addr] == FREE_VALUE) {
      printf("Invalid physical address\n");
      return POISON_VALUE;
   }
   
   if (list->size == list->maxsize) ExpandList(list);
      

   //analyse free list
   int free_value = list->free;
   list->size += 1;

   if (list->size == list->maxsize)
      list->free = POISON_VALUE;
   else
      list->free = list->next[free_value];
   
   //adding element
   list->data[free_value] = value;
   list->prev[free_value] = list->prev[physical_addr];
   list->next[free_value] = physical_addr;
   
   //edit prev[physical_addr]
   if (physical_addr != list->head)
      list->next[list->prev[physical_addr]] = free_value;

   //edit physical_addr
   list->prev[physical_addr] = free_value;
   
   if (physical_addr == list->head)
      list->head = free_value;
   
   list->sorted = 0;

   return free_value;
}

int ListSoftPushBack(List* list, int physical_addr, Elem_t value) {
   assert(ListValid(list));
   assert(physical_addr > 0);
   assert(list->sorted);
   assert(physical_addr == list->tail);

   if (list->prev[physical_addr] == FREE_VALUE) {
      printf("Invalid physical address\n");
      return POISON_VALUE;
   }
   
   if (list->size == list->maxsize) ExpandList(list);
   
   //analyse new value
   list->data[physical_addr + 1] = value;
   list->next[physical_addr + 1] = 0;
   list->prev[physical_addr + 1] = list->tail;
   
   //analyse list
   list->next[list->tail] = list->tail + 1;
   list->tail += 1;
   list->size += 1;

   if (list->size == list->maxsize)
      list->free = POISON_VALUE;
   else
      list->free = list->next[list->free];
   
   return 1;
}


int ListAddAfter(List* list, int physical_addr, Elem_t value) {
   assert(ListValid(list));
   assert(physical_addr > 0);
  
   if (list->prev[physical_addr] == FREE_VALUE) {
      printf("Invalid physical address\n");
      return POISON_VALUE;
   }
   
   if (list->size == list->maxsize) ExpandList(list);
   
   //if sorted
   if (list->sorted && physical_addr == list->tail) {
      return ListSoftPushBack(list, physical_addr, value);
   }
      
   //analyse free list
   int free_value = list->free;
   list->size += 1;

   if (list->size == list->maxsize)
      list->free = POISON_VALUE;
   else
      list->free = list->next[free_value];
   
   //adding element
   list->data[free_value] = value;
   list->prev[free_value] = physical_addr;
   list->next[free_value] = list->next[physical_addr];

   //edit next[physical_addr]
   if (physical_addr != list->tail)
      list->prev[list->next[physical_addr]] = free_value;

   //edit physical_addr
   list->next[physical_addr] = free_value;
   
   if (physical_addr == list->tail)
      list->tail = free_value;
   
   list->sorted = 0;

   return free_value;
}

int ListPushBack(List* list, Elem_t value) {
   assert(ListValid(list));
   
   //if no elements in the list
   if (list->tail == 0) {
      
      //analyse free list
      int free_value = list->free;
      list->size += 1;
      list->free = list->next[free_value];
      
      //analyse first element
      list->data[free_value] = value;
      list->next[free_value] = POISON_VALUE;
      list->prev[free_value] = POISON_VALUE;

      list->head = free_value;
      list->tail = free_value;

      return free_value;
   }
   
   if (list->size == list->maxsize) ExpandList(list);

   //analyse free list
   int free_value = list->free;
   list->size += 1;

   if (list->size == list->maxsize)
      list->free = POISON_VALUE;
   else
      list->free = list->next[free_value];
   
   //adding element
   list->data[free_value] = value;
   list->prev[free_value] = list->tail;
   list->next[free_value] = POISON_VALUE;

   //edit tail
   list->next[list->tail] = free_value;
   list->tail = free_value;

   return free_value;
}

int ListSoftDelete (List* list, int physical_addr) {
   assert(ListValid(list));
   assert(physical_addr == list->tail);
   assert(list->sorted == 1);
   
   if (list->size == 0) {
      printf("List is empty\n");
      return -1;
   }

   if (list->prev[physical_addr] == FREE_VALUE) {
      printf("Element is already free\n");
      return POISON_VALUE;
   }
   list->size -= 1;

   list->next[list->prev[physical_addr]] = POISON_VALUE;
   list->tail = list->prev[physical_addr];
   
   //add new free element
   list->data[physical_addr] = 0;
   list->prev[physical_addr] = FREE_VALUE;
   list->next[physical_addr] = list->free;
   list->free = physical_addr;
   
   list->sorted = 1;

   return physical_addr;
}

int ListDelete(List* list, int physical_addr) {
   assert(ListValid(list));
   assert(physical_addr > 0);
   
   if (list->size == 0) {
      printf("List is empty\n");
      return -1;
   }

   if (list->prev[physical_addr] == FREE_VALUE) {
      printf("Element is already free\n");
      return POISON_VALUE;
   }
   
   if (list->sorted == 1 && physical_addr == list->tail)
      ListSoftDelete(list, physical_addr);

   list->size -= 1;

   if (physical_addr != list->head)
      list->next[list->prev[physical_addr]] = list->next[physical_addr];
      
   if (physical_addr != list->tail)
      list->prev[list->next[physical_addr]] = list->prev[physical_addr];

   //if we delete head or tail
   if (physical_addr == list->head)
      list->head = list->next[physical_addr];

   if (physical_addr == list->tail)
      list->tail = list->prev[physical_addr];
   
   //add new free element
   list->data[physical_addr] = 0;
   list->prev[physical_addr] = FREE_VALUE;
   list->next[physical_addr] = list->free;
   list->free = physical_addr;
   
   list->sorted = 0;

   return physical_addr;
}

void PrintList(List* list, int maxsize) {
//   assert(ListValid(list));
//   assert(maxsize <= list->maxsize + 1);
   
   printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
   for (int i = 0; i < maxsize; ++i)
      printf("%3d ", i);
   printf("\n");

   for (int i = 0; i < maxsize; ++i)
      printf("%3" LIST_SYMBOL " ", list->data[i]);
   printf("\n");

   for (int i = 0; i < maxsize; ++i)
      printf("%3d ", list->next[i]);
   printf("\n");

   for (int i = 0; i < maxsize; ++i)
      printf("%3d ", list->prev[i]);
   printf("\n\n");

   PRINT_MAGENTA(BOLD("list->tail ="));    printf(" %" LIST_SYMBOL "\n", list->tail);
   PRINT_MAGENTA(BOLD("list->head ="));    printf(" %" LIST_SYMBOL "\n", list->head);
   PRINT_MAGENTA(BOLD("list->free ="));    printf(" %" LIST_SYMBOL "\n", list->free);
   PRINT_MAGENTA(BOLD("list->size ="));    printf(" %lu\n", list->size);
   PRINT_MAGENTA(BOLD("list->maxsize =")); printf(" %lu\n", list->maxsize);


   printf("--------------------------------------------------\n");

}

int SearchByElem(List* list, Elem_t value) {
   assert(ListValid(list));

   if (list->size == 1)
      return list->data[list->head] == value;

   for (int it = list->head; it != list->tail; it = list->next[it])
      if (list->data[it] == value)
         return it;

   return POISON_VALUE;
}

int SearchByLogicAddres(List* list, int logic_address) {
   assert(ListValid(list));
   
   Assert(logic_address > 0);
   Assert(logic_address <= list->maxsize);
   
   if (list->sorted)
    return logic_address;

   int it = list->head;
   logic_address--;

   while (logic_address != 0) {
      
      it = list->next[it];
      logic_address--;
   }

   return it;
}

bool SortList(List* list) {
   assert(ListValid(list));
   
   if (list->size == 0) { list->sorted = 1; return 1; }

   //sorting data
   Elem_t* new_data = (Elem_t*)calloc(list->maxsize + 1, sizeof(Elem_t));
   new_data[0] = POISON_VALUE;
   
   for (int i = list->head, j = 1; i != list->tail; i = list->next[i], ++j)
      new_data[j] = list->data[i];
   new_data[list->size] = list->data[list->tail];

   free(list->data);
   list->data = new_data;

   //prev and next
   for (int i = 1; i < list->size + 1; ++i) {
      list->next[i] = i + 1;
      list->prev[i] = i - 1;

   }
   list->next[list->size] = POISON_VALUE;
   
   //other
   list->head = 1;
   list->tail = list->size;
   
   //free values

   if (list->size == list->maxsize) {
      list->free = POISON_VALUE;
      
      list->sorted = 1;
      return 1;
   }

   for (int i = list->size + 1; i < list->maxsize + 1; ++i) {
      list->prev[i] = POISON_VALUE;
      list->next[i] = i + 1;
   }
   
   list->free = list->size + 1;
   list->next[list->maxsize] = POISON_VALUE;

   list->sorted = 1;
   return 1;
}
