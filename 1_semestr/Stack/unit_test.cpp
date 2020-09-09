#include "stack.cpp"

void unit_test0(Stack_t* stack);
void unit_test1();

int main() {
   
   Stack_t stack = {};
   StackInit(&stack, 3);
   unit_test0(&stack);
   unit_test1();
   StackDisinit(&stack);
   
   unit_test1();

   return 0;
}

void unit_test0(Stack_t* stack) {

   for (int i = 0; i < 1000; ++i)
      StackPush(stack, i);

   bool error = 0;
   for (int i = 0; i < 1010; ++i)
      StackPop(stack, &error);

   for (int i = 0; i < 100; ++i)
      StackPush(stack, i);

}

void unit_test1() {
   Stack_t stack = {};
   StackInit(&stack);
   stack.data[2] = 2;
   StackPush(&stack, 3);
   StackDisinit(&stack);
   printf("test1\n");

   stack = {};
   StackInit(&stack);
   stack.size = 1000;
   StackPush(&stack, 3);
   StackDisinit(&stack);
   printf("test2\n");
   
   stack = {};
   StackInit(&stack);
   stack.number_elem = 100;
   StackPush(&stack, 3);
   StackDisinit(&stack);
   printf("test3\n");
   
   stack = {};
   StackInit(&stack);
   stack.hash = 14;
   StackPush(&stack, 3);
   StackDisinit(&stack);
   printf("test4\n");
   
   stack = {};
   StackInit(&stack);
   stack.data[15] = 3;
   StackPush(&stack, 3);
   StackDisinit(&stack);
   printf("test5\n");
   
   stack = {};
   StackInit(&stack);
   stack.data[15] = 1000000;
   StackPush(&stack, 3);
   StackDisinit(&stack);
   printf("test6\n");
   
}
