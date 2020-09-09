#include "../Headers/stack.h"

bool StackInit(Stack_t* stack, size_t stack_size ) {
   assert(stack != NULL);
   assert(stack_size >= 0);

   stack->size        = 0;
   stack->number_elem = stack_size;
   stack->hash        = 0;
   stack->chicken2    = KANARY_CONST;
   stack->chicken1    = KANARY_CONST;
   stack->data        = (char*) calloc(stack->number_elem * sizeof(Elem_t) + sizeof(Kanary_t) * 2, sizeof(char));
   if(stack->data == NULL)
      return 0;
   
   //Kanary for data
   *(Kanary_t*)(stack->data) = KANARY_CONST;
   *(Kanary_t*)(stack->data + sizeof(Kanary_t) + sizeof(Elem_t) * stack->number_elem) = KANARY_CONST;
   
   //filling data with POISON_VALUE
   for (size_t i = 0; i < stack->number_elem; ++i)
      ((Elem_t*)(stack->data + sizeof(Kanary_t)))[i] = POISON_VALUE;

   stack->hash = StackHash(stack);
   
   Assert(stack);
   return 1;
}

void StackDisinit(Stack_t* stack) {
   Assert(stack);

   free(stack->data);
   stack = NULL;
}

unsigned long long StackHash(Stack_t* stack) {
   assert(stack != NULL);
   
   stack->hash = 0;
   unsigned long long hash = Hash(stack, sizeof(Stack_t)) + \
                             Hash((stack->data), sizeof(Kanary_t) * 2 + stack->number_elem * sizeof(Elem_t));
   return hash;
}

unsigned long long Hash(const void* const struc, const size_t nbytes) {
   assert(struc != NULL);
   assert(nbytes > 0);
   
   const unsigned char* const iter = (const unsigned char*) struc;
   unsigned long long hash = 0;
   
   for (size_t i = 0; i < nbytes; ++i) {
      hash += hash * 33 + iter[i];
   }

   return hash;
}

void IsStackOk(Stack_t* stack, const char* file, int line, const char* function) {
  return;

  if (LEVEL_SECURITY == 1) {
      soft_assert(stack != NULL);
      soft_assert(stack->data != NULL);
      soft_assert(stack->size <= stack->number_elem);
   }

   if (LEVEL_SECURITY == 0) {
      hard_assert(stack != NULL);
      hard_assert(stack->data != NULL);
      hard_assert(stack->size <= stack->number_elem);
      hard_assert(stack->chicken1 == KANARY_CONST);
      hard_assert(stack->chicken2 == KANARY_CONST);
      hard_assert(*((Kanary_t*)(stack->data)) == KANARY_CONST);
      hard_assert(*((Kanary_t*)(stack->data + sizeof(Kanary_t) + sizeof(Elem_t) * stack->number_elem)) == KANARY_CONST);

      //checking poison values
      for (size_t i = stack->size; i < stack->number_elem; ++i)
         hard_assert(((Elem_t*)(stack->data + sizeof(Kanary_t)))[i] - POISON_VALUE + 0.00000000001 > 0);

      //checking hash
      unsigned long long old_hash = stack->hash;
      hard_assert(old_hash == StackHash(stack))
      stack->hash = old_hash;
   }
}

bool StackPush(Stack_t* stack, Elem_t value) {
   Assert(stack);
   
   //expand if need
   if (stack->size >= stack->number_elem) 
      if( ! ExpandStack(stack))
         return 0;
   
   //pushing
   ((Elem_t*)(stack->data + sizeof(Kanary_t)))[stack->size] = value;
   (stack->size)++;

   //make new hash
   stack->hash = StackHash(stack);

   return 1;
}

bool ExpandStack(Stack_t* stack) {
   Assert(stack);
   
   //realloc data
   char* temp_ptr = (char*) realloc(stack->data, stack->number_elem * sizeof(Elem_t) * EXPAND_COEF + sizeof(Kanary_t) * 2);
  
   if (temp_ptr  == NULL)
      return 0;
   

   stack->data        = temp_ptr;
   stack->number_elem = stack->number_elem * EXPAND_COEF;

   //fill new elements with POISON_VALUE
   for (size_t i = stack->size; i < stack->number_elem; ++i)
      ((Elem_t*)(stack->data + sizeof(Kanary_t)))[i] = POISON_VALUE;

   *(Kanary_t*)(stack->data) = KANARY_CONST;
   *(Kanary_t*)(stack->data + sizeof(Kanary_t) + sizeof(Elem_t) * stack->number_elem) = KANARY_CONST;

   //recompute hash
   stack->hash = StackHash(stack);

   return 1;
}

//error = 0 - no error
//error = 1 - can not squeeze stack
//error = 2 - stack is empty
Elem_t StackPop(Stack_t* stack, int* error) {
   Assert(stack);

   *error = 0; // no error by default

   //stack empty?
   if (stack->size == 0) {
      *error = 2;
      return 0;
   }
   
   //popping element
   Elem_t value = ((Elem_t*)(stack->data + sizeof(Kanary_t)))[stack->size-1];
   stack->size--;

   //filling new value with POISON_VALUE
   ((Elem_t*)(stack->data + sizeof(Kanary_t)))[stack->size] = POISON_VALUE;

   //recompute hash
   stack->hash = 0;
   stack->hash = StackHash(stack);
  
   //squeeze stack, if need
   if (stack->size != 0)
   if (stack->number_elem / stack->size > 10)
      if ( ! SqueezeStack(stack)) {
         *error = 1;
         return 0;
      }

   return value;
}

bool SqueezeStack(Stack_t* stack) {
   Assert(stack);

   //realloc data
   char* temp_ptr = (char*) realloc(stack->data, (stack->number_elem * sizeof(Elem_t) / EXPAND_COEF) + sizeof(Kanary_t) * 2);

   if (temp_ptr  == NULL)
      return 0;

   stack->data        = temp_ptr;
   stack->number_elem = stack->number_elem / EXPAND_COEF;

   *(Kanary_t*)(stack->data) = KANARY_CONST;
   *(Kanary_t*)(stack->data + sizeof(Kanary_t) + sizeof(Elem_t) * stack->number_elem) = KANARY_CONST;
  
   //recompute hash
   stack->hash = 0;
   stack->hash = StackHash(stack);

   Assert(stack);
   return 1;
}

void StackDump(Stack_t* stack, const char reason[], const char* file, int line, const char* function, bool stack_ok = 1){
   assert(stack != NULL);
   
   //reason
   if (reason != NULL) {
     PRINT_RED_E(BOLD("\n%s\n---------------------------------------------------------\n"), reason);
   }


   //Where are from the dump
   printf("Dump from the %s:%d, function:%s\n",file, line, function);
   
   //Stack header
   printf("Stack_t ");

   PRINT_MAGENTA_E(N("[0x%p] "), stack);
   if (stack_ok) {
      PRINT_GREEN(N("(ok)"));
      printf("\n{\n   ");
   }
   else {
      PRINT_RED(BLINK("(ERROR)"));
      printf("\n{\n   ");
   }
      
    
   //Stack variables
   PRINT_ORANGE(N("chicken1")); 
   printf(" = 0x%Ld\n   ", stack->chicken1);

   PRINT_ORANGE(N("size")); 
   printf(" = %lu\n   ", stack->size);
    
   PRINT_ORANGE(N("data"));
   printf(" [%lu] ", stack->number_elem);
   PRINT_MAGENTA_E(N("[0x%p]"), stack->data);
   
   //data elements
   if (stack->data != NULL) {

      printf(" {\n   ");
      PRINT_ORANGE(N("   kurlik1"));
      printf(" = 0x%Ld\n   ", *(Kanary_t*)(stack->data));
      
      for (size_t i = 0; i < stack->number_elem; ++i) {
         if (i < stack->size) printf("   * ");
         else                 printf("     ");

         printf("[%lu] = " STACK_SYMBOL, i, ((Elem_t*)(stack->data + sizeof(Kanary_t)))[i]);
      
         if (((Elem_t*)(stack->data + sizeof(Kanary_t)))[i] - POISON_VALUE + 0.000000001 > 0)  PRINT_GREEN(N(" (POISON)\n   "));
         else                                                                 printf("\n   ");
      }

      PRINT_ORANGE(N("   kurlik2"));
      printf(" = 0x%Ld\n   ", *(Kanary_t*)(stack->data + sizeof(Kanary_t) + sizeof(Elem_t) * stack->number_elem));
      printf("}\n   ");
   }
   
   //Stack variables
   PRINT_ORANGE(N("number_elem"));
   printf(" = %lu\n   ", stack->number_elem);

   PRINT_ORANGE(N("hash"));
   printf(" = %Lu\n   ", stack->hash);


   PRINT_ORANGE(N("chicken2"));
   printf(" = 0x%Ld\n}\n", stack->chicken2);

   PRINT_CYAN(BOLD("---------------------------------------------------------\n"));
}

void PrintStack(Stack_t* stack) {
   Assert(stack);
   
   PRINT_CYAN(BOLD("--------------------Stack start--------------------------\n"));
   printf("Number elem: %lu  Size: %lu\n\n", stack->number_elem, stack->size);
   for (size_t i = 0; i < stack->number_elem; ++i) {
      if (i < stack->size) printf(" * ");
      else                 printf("   ");

      printf("[%lu] = " STACK_SYMBOL, i, ((Elem_t*)(stack->data + sizeof(Kanary_t)))[i]);
    
      if (((Elem_t*)(stack->data + sizeof(Kanary_t)))[i] - POISON_VALUE + 0.00000000001 > 0) PRINT_GREEN(DIM(" (POISON)\n"));
      else                                                                printf("\n");
   }
   PRINT_CYAN(BOLD("--------------------Stack end----------------------------\n"));
}   
