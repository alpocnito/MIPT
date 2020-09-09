#include "list.cpp"

void test1() {

   List list = {};
   ListInit(&list, 3);
   
   for (int i = 0; i < 1000; ++i)
      ListPushBack(&list, i);

   for (int i = 1; i < 100; ++i)
      ListDelete(&list, i);
   
   PrintList(&list, 10);
   ListDisinit(&list);
}

void test2() {
    
   List list = {};
   ListInit(&list, 3);
   
   
   SortList(&list);
   PrintList(&list, list.maxsize + 1);


   ListPushBack(&list, 10);


   SortList(&list);
   PrintList(&list, list.maxsize + 1);


   int test = ListPushBack(&list, 20);
   ListPushBack(&list, 30);
   
   
   SortList(&list);
   PrintList(&list, list.maxsize + 1);
      
   
   ListAddBefore(&list, test, 15);
   ListAddAfter (&list, test, 25);
   

   SortList(&list);
   PrintList(&list, list.maxsize + 1);
   

   ListDelete(&list, test);
   ListDelete(&list, 1);
   

   SortList(&list);
   PrintList(&list, list.maxsize + 1);


   ListPushBack(&list, 50);
   ListPushBack(&list, 60);
   
   
   SortList(&list);
   PrintList(&list, list.maxsize + 1);
   

   printf("%d\n", SearchByElem(&list, 15));
   printf("%d\n", SearchByLogicAddres(&list, 5));

   ListDisinit(&list);
}

void test3() {
   
   List list = {};
   ListInit(&list, 3);

   int test = ListPushBack(&list, 10);

   for (int i = 0; i < 1000; ++i) {
      
      ListAddBefore(&list, test, i);
      ListAddAfter(&list, test, i);
      
   }

   PrintList(&list, 10);
  
   ListDisinit(&list);
}

int main() {

   test1(); printf("\n\n\n\n\n");
   test2(); printf("\n\n\n\n\n");
   test3(); printf("\n\n\n\n\n");

   return 0;
}
