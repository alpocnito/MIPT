#define OAssert(expr) if ((expr) == 0) {                   \
                                                          \
   PRINT_RED(BOLD(#expr "   is not true\n"));             \
   printf("In %s:%d\n", __PRETTY_FUNCTION__, __LINE__);   \
   return 0;                                              \
} 

#define OList_SYMBOL "s"
typedef char* OElem_t;

//////////////////////////////// OList //////////////////////////////
struct OListNode {
  
  OElem_t data;
  OListNode* next;
};

struct OList {
  
  OListNode* first_elem;
  size_t number_nodes;
  OListNode* last_elem;
};

OListNode memory[66927810];

////////////////////////////////////////////////////////////////////

int OListValid  (OList* OList);
void PrintOList (OList* OList);

void OListInit    (OList* OList);
void OListDisinit (OList* OList);

int OListPush   (OList* OList, OElem_t value);
int OListDelete (OList* OList, int number_node);

int OListValid(OList* OList) {
  return 1;
}


void OListInit(OList* OList) {
  OAssert(OList != NULL);

  OList->first_elem = NULL;
  OList->last_elem  = NULL;
  OList->number_nodes = 0;
}

void OListDisinit(OList* OList) {
  //OList = 0;
  OAssert(OListValid(OList));
  
  OListNode* next_elem = OList->first_elem;
  OListNode* prev_elem = NULL;
  for (int i = 0; i < OList->number_nodes; ++i) {
    
    prev_elem = next_elem;
    next_elem = next_elem->next;
//    free(prev_elem);
  }
}

int OListPush(OList* OList, OElem_t value) {
  static int number_calls = 0;
  //printf("%d\n", ++number_calls);  
  //OListNode* new_node = (OListNode*)calloc(1, sizeof(OListNode));
  OListNode* new_node = memory + number_calls++;
 
  //init node
  new_node->data = value;
  new_node->next = NULL;
  
  if (OList->number_nodes == 0) {
    OList->first_elem = OList->last_elem = new_node;
  }
  else {
    OList->last_elem->next = new_node;
    OList->last_elem = new_node;
  }
  OList->number_nodes += 1;
  
  return 0;
}

int OListDelete(OList* OList, int number_node) {
  OAssert(OListValid(OList));
  
  if (number_node >= OList->number_nodes)
    return 1;
  
  //if we delete first element
  if (number_node == 0) {
    
    OListNode* new_first_elem = OList->first_elem->next;
    //free(OList->first_elem);
    OList->first_elem = new_first_elem;

    OList->number_nodes -= 1;
    return 0;
  }

  OListNode* prev    = OList->first_elem;
  OListNode* current = OList->first_elem->next;
  for (int i = 1; i < number_node; ++i) {
    
    current = current->next;
    prev    = prev->next;
  }
  
  OListNode* new_current = current->next;
  //free(current);
  prev->next = new_current;

  OList->number_nodes -= 1;
  
  //if we delete last element
  if (new_current == NULL)
    OList->last_elem = prev;
  
  return 0;
}

void PrintOList(OList* OList) {
  OAssert(OList != NULL);

  for (OListNode* it_node = OList->first_elem; it_node != NULL; it_node = it_node->next)
    printf("%" OList_SYMBOL ",", it_node->data);
  printf("\n");

  //PRINT_MAGENTA(N("first_elem   = ")); printf("%" OList_SYMBOL "\n", OList->first_elem->data);
  //PRINT_MAGENTA(N("number_nodes = ")); printf("%lu\n", OList->number_nodes);
  //PRINT_MAGENTA(N("last_elem    = ")); printf("%" OList_SYMBOL "\n", OList->last_elem->data);
}



typedef unsigned long long OHash_t;
const int OTABLE_SIZE = 18061;

struct OHashTable {
  OList** OHash_array;
};


OHash_t OHash_First  (void* buffer, size_t buffer_size);
OHash_t OHash_Sum    (void* buffer, size_t buffer_size);
OHash_t OHash_Len    (void* buffer, size_t buffer_size);
OHash_t OHash_Rotate (void* buffer, size_t buffer_size);
OHash_t OHash_GNU    (void* buffer, size_t buffer_size);
OHash_t OHash_Rot13  (void* buffer, size_t buffer_size);
OHash_t OHash_FAQ6   (void* buffer, size_t buffer_size);
OHash_t OHash_Ly     (void* buffer, size_t buffer_size);
OHash_t OHash_Rs     (void* buffer, size_t buffer_size);


int  OHashTableValid   (OHashTable* htable);
int  OHashTableInit    (OHashTable* htable);
void OHashTableDisinit (OHashTable* htable);

int OHashTablePush (OHashTable* htable, char* value, OHash_t (*OHash)(void* buffer, size_t buffer_size)) noexcept;



int OHashTableValid(OHashTable* htable) {
  
  return 1;
}

int OHashTableInit(OHashTable* htable) {
  OAssert(htable != NULL);
  
  htable->OHash_array = (OList**) calloc(OTABLE_SIZE, sizeof(OList*));
  if (htable->OHash_array == NULL)
    return 0;

  //fill OHash array
  for (int i = 0; i < OTABLE_SIZE; ++i) {
    htable->OHash_array[i] = (OList*)calloc(1, sizeof(OList));
    if (htable->OHash_array[i] == NULL)
      return 0;

    OListInit(htable->OHash_array[i]);
  }
  
  return 1;
}

void OHashTableDisinit(OHashTable* htable) {
  OAssert(OHashTableValid(htable));

  for (int i = 0; i < OTABLE_SIZE; ++i) {
    OListDisinit(htable->OHash_array[i]);
    free(htable->OHash_array[i]);
  }
  
  free(htable->OHash_array);
}

//------------------------------------------------------------------------------------------
//
// Return 0, if value wasn't pushed
// Return 1, if Value was pushed
// Return 2, if value is already exist
//
//------------------------------------------------------------------------------------------
int OHashTablePush(OHashTable* htable, char* value, OHash_t (*OHash)(void* buffer, size_t buffer_size)) noexcept {
  OHash_t OHash = (*OHash)(value, strlen(value));
  OList* OList = htable->OHash_array[OHash % OTABLE_SIZE];

  //creating new OListNode
  if (OListPush(OList, value)) return 0;
  
  return 1;
}




//////////////////////////// OHash /////////////////////////////////////////////////////////////////////////////////////////
OHash_t OHash_First(void* buffer, size_t buffer_size) {
  OAssert(buffer != NULL);
  OAssert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;

  return (unsigned char)norm_buffer[0];
}

OHash_t OHash_Len(void* buffer, size_t buffer_size) {
  OAssert(buffer != NULL);
  OAssert(buffer_size > 0);

  return buffer_size;
}

OHash_t OHash_Sum(void* buffer, size_t buffer_size) {
  OAssert(buffer != NULL);
  OAssert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  OHash_t OHash = 0;

  for (int i = 0; i < buffer_size; ++i)
    OHash += (unsigned char)norm_buffer[i];

  return OHash;
}

OHash_t OHash_Rotate(void* buffer, size_t buffer_size) {
  OAssert(buffer != NULL);
  OAssert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  OHash_t OHash = 0;
  
  
  for (int i = 0 ; i < buffer_size; ++i) {
    OHash = (OHash << 4) ^ (unsigned char)norm_buffer[i];
  }
  return OHash;
}

OHash_t OHash_GNU(void* buffer, size_t buffer_size) {
  OAssert(buffer != NULL);
  OAssert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  OHash_t OHash = 0;

  for (int i = 0; i < buffer_size; ++i)
    OHash = (OHash << 5  + OHash) + (unsigned char)norm_buffer[i];
  
  return OHash;
}

OHash_t OHash_FAQ6(void* buffer, size_t buffer_size) {
  OAssert(buffer != NULL);
  OAssert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  OHash_t OHash = 0;

  for (int i = 0; i < buffer_size; ++i) {
    OHash += (unsigned char)norm_buffer[i];
    OHash += OHash << 10;
    OHash ^= OHash >> 6;
  }
  OHash += OHash << 3;
  OHash ^= OHash >> 11;
  OHash += OHash << 15;

  return OHash;
}

OHash_t OHash_Rot13(void* buffer, size_t buffer_size) {
  OAssert(buffer != NULL);
  OAssert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  OHash_t OHash = 0;

  for (int i = 0; i < buffer_size; ++i) {
    OHash += (unsigned char)norm_buffer[i];
    OHash -= (OHash << 13) | (OHash >> 19);
  }

  return OHash;
}

OHash_t OHash_Ly(void* buffer, size_t buffer_size) {
  OAssert(buffer != NULL);
  OAssert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  OHash_t OHash = 0;

  for (int i = 0; i < buffer_size; ++i)
    OHash = OHash * 1664525 + (unsigned char)norm_buffer[i] + 1013904223;

  return OHash;
}

OHash_t OHash_Rs(void* buffer, size_t buffer_size) {
  OAssert(buffer != NULL);
  OAssert(buffer_size > 0);

  char* norm_buffer = (char*) buffer;
  OHash_t OHash = 0;
  
  static const unsigned int b = 378551;
  unsigned int a = 63689;
  for (int i = 0; i < buffer_size; ++i) {
    OHash = OHash * 63689 + (unsigned char)norm_buffer[i];
    a *= b;
  }

  return OHash;
}



const int NUMBER_OHashES    = 9;
const int OMAX_FILE_NAME    = 10;
const int OMAX_NUMBER_FILES = 20;

//-----------------------------------------------------------
//
//  Function run an array OList_of_lines with number lines = 
//    number_lines through OHash = (*OHash) and write a vertical
//    line into file = file_name
//
//-----------------------------------------------------------
int ORunTest(OHash_t (*OHash)(void* buffer, size_t buffer_sizei), String* \
            OList_of_lines, int number_lines, const char* file_name) {
  
  OHashTable htable = {}; if (!OHashTableInit(&htable)) return 0;
  
  // computing OHash
  for (int i = 0; i < number_lines; ++i)
    OHashTablePush(&htable, OList_of_lines[i].str, OHash);

  OHashTableDisinit(&htable);
  return 1;
}
 
int OGetLines(const char* file_name, String** OList_of_lines, char** buffer) {

  // reading dict
  int buffer_size = ReadFile(file_name, buffer);
  int number_lines = SeparateLines(*buffer, buffer_size, OList_of_lines);
  NormalizeStrings(OList_of_lines, number_lines);

  return number_lines;
}

int old_main() {
  
  char*   buffer        = NULL;
  String* OList_of_lines = NULL;
  int number_lines = OGetLines("dict1", &OList_of_lines, &buffer);

  // name of files
  char* name_of_result_files[NUMBER_OHashES] = {};
  for (int i = 0; i < NUMBER_OHashES; ++i) {
    name_of_result_files[i] = (char*) calloc(OMAX_FILE_NAME, sizeof(char));
    sprintf(name_of_result_files[i], "%d", i);
  }
  for (int i = 0; i < 20; ++i) {    
  ORunTest(OHash_First,  OList_of_lines, number_lines, name_of_result_files[0]); 
  ORunTest(OHash_Sum,    OList_of_lines, number_lines, name_of_result_files[1]);
  ORunTest(OHash_Len,    OList_of_lines, number_lines, name_of_result_files[2]);
  ORunTest(OHash_Rotate, OList_of_lines, number_lines, name_of_result_files[3]); 
  ORunTest(OHash_GNU,    OList_of_lines, number_lines, name_of_result_files[4]); 
  ORunTest(OHash_Rot13,  OList_of_lines, number_lines, name_of_result_files[5]); 
  ORunTest(OHash_FAQ6,   OList_of_lines, number_lines, name_of_result_files[6]);
  ORunTest(OHash_Ly,     OList_of_lines, number_lines, name_of_result_files[7]); 
  ORunTest(OHash_Rs,     OList_of_lines, number_lines, name_of_result_files[8]);
  }
 /* merge(name_of_result_files[0] ,\
        name_of_result_files[1] ,\
        name_of_result_files[2] ,\
        name_of_result_files[3] ,\
        name_of_result_files[4] ,\
        name_of_result_files[5] ,\
        name_of_result_files[6] ,\
        name_of_result_files[7] ,\
        name_of_result_files[8] ,\
        NULL                    );
  */
  free(OList_of_lines);
  free(buffer);
  return 0;
}

