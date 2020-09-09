#include <stdio.h>
#include "tree.cpp"
#include "color.h"

#define MIN(first, second) if ((first) < (second)) \
                              return first;        \
                           else                    \
                              return second;       
//#define SAY(sentence) system("echo \"" #sentence "\" | festival --tts");
#define SAY(sentence) ;







////////////////////////////////////////////////////////////////////////////////
//
//                              Greed function
//
////////////////////////////////////////////////////////////////////////////////
void GreedFunction(void) {
  SAY("Hello, my friend. I am acinator. Read the instruction");
  system("clear");

  printf("                           ");
  PRINT_GREEN(REVERSE("AKINATOR\n\n\n"));

    PRINT_WHITE(N("Akinator is a computer game. \n\
      During gameplay, it attempts to determine what University or faculty the player \n\
      is thinking of by asking a series of questions. \n\
      It uses an artificial intelligence program that learns the best questions \n\
      to ask through its experience with players\n\n"));
  
  PRINT_CYAN(BOLD("~~~~~~~~~~~~~~~~~~~~~~You can~~~~~~~~~~~~~~~~~~~~~~\n\n"));
  PRINT_CYAN(N(" save  - Save tree. In case, that something is wrong\n"));
  PRINT_CYAN(N(" add   - Function to start guessing\n"));
  PRINT_CYAN(N(" diff  - Give difference between two universities\n"));
  PRINT_CYAN(N(" print - Print tree\n"));
  PRINT_CYAN(N(" end   - End program\n\n"));
  
}


////////////////////////////////////////////////////////////////////////////////
//
//                              Init fuction
//
// how_it_is - sign for the root
// he_is_not - left leaf of the root
// he_is     - right leaf of the root
//
////////////////////////////////////////////////////////////////////////////////
int AcinatorTreeInit(Tree* tree, char* how_it_is, char* he_is_not, char* he_is) {
  
  assert(!TreeInit(tree));
  assert(!TreePush(tree, &(tree->root), how_it_is));
  assert(!TreePush(tree, &(tree->root->left),  he_is_not));
  assert(!TreePush(tree, &(tree->root->right), he_is));

  return 1;
}


////////////////////////////////////////////////////////////////////////////////
//
//                              Push fuction
// node - there to push
// 
// how_it_is - node
// he_is_not - left leaf of the node
// he_is     - right leaf of the node
//
////////////////////////////////////////////////////////////////////////////////
int AcinatorTreePush(Tree* tree, Node** node, char* how_it_is, char* he_is_not, char* he_is) {
  assert(node);
  assert(*node);

  (*node)->data = how_it_is;
  assert(!TreePush(tree, &((*node)->left),  he_is_not));
  assert(!TreePush(tree, &((*node)->right), he_is));
  
  return 1;
}











///////////////////////////// Helpful functions ///////////////////////////////
char** AcGivePath(Tree* tree, char* node, size_t* path_size);


////////////////////////////////////////////////////////////////////////////////
//
//                              Check fuction
//                  Write to the stdout, if leaf exist
//
// new_node - node to check
//
////////////////////////////////////////////////////////////////////////////////
int AcinatorCheckNode(Tree* tree, char* new_node) {
  assert(tree);
  assert(new_node);
  
  size_t path_size = 0;
  char** path_to_new_node = AcGivePath(tree, new_node, &path_size);  
  if (path_to_new_node == NULL) return 0;

  // Giving path  
  SAY("No, you are wrong!!!");
  PRINT_RED(BOLD("No, you are wrong!!!\n"));
  printf("%s don't have this signs!!!!\nHe is ", new_node);
  
  for (int i = path_size - 1; i >= 0; --i) {
    int error = 0;
    char* sign = path_to_new_node[i];

    if (i > 0) printf("%s, ", sign);
    else       printf("%s\n", sign);

    free(path_to_new_node[i]);
  }
  
  free(path_to_new_node);
  return 1;
}

///////////////////////////// Helpful functions ///////////////////////////////
char** AcGivePath(Tree* tree, char* node_data, size_t* path_size) {
  assert(tree);
  assert(node_data);
  assert(path_size);
  
  Stack_t stack = {};
  if (!TreeFindElement(tree, node_data, &stack))
    return NULL;

  // new_node already exist, let's say it
  //
  // Finding path
  char** path_to_node = (char**) calloc(stack.number_elem + 1, sizeof(char*));
  size_t stack_size = stack.size;
         *path_size = stack.size;

  for (int i = 0; i < stack_size; ++i) {
    
    int error = 0;
    path_to_node[i] = StackPop(&stack, &error);
  }
  
  return path_to_node;
}













///////////////////////////// Helpful functions ///////////////////////////////
char* AcScanfAnswer();
void AcAddNewNode(Tree* tree, char* new_node, Node** node);


///////////////////////////////////////////////////////////////////////////////
//
//                              Find function
//               Main function for finding and adding nodes
//
// node - there to find
//
////////////////////////////////////////////////////////////////////////////////
int AcinatorFind(Tree* tree, Node** node) {
  assert(tree);
  assert(node);
  

  // Take answer
  if ((*node)->left == NULL) {
    printf("You are from: ");
    PRINT_MAGENTA_E(N("%s?\n"), (*node)->data);
  }
  else {
    printf("It is about you: ");
    PRINT_MAGENTA_E(N("%s?\n"), (*node)->data);
  }

  printf("Write yes/no\nAnswer:");
  char* answer = AcScanfAnswer();  

  // Leaf of the tree => we found it!
  if ((*node)->left == NULL) {
    
    if (answer[0] == 'y') {
      printf("Haha, you see, I am very smart. Give me some time, and world be mine!!!!\n");
      SAY("Haha, you see, I am very smart. Give me some time, and world be mine!!!!");
      printf("------------------------------------------------------------------------\n\n");
    }
    else {
      printf("Hmmmm, very strange, where are you from?\nAnswer:");
      SAY("Hm, very strange, where are you from?");
      
      // create new node
      char* new_node = strdup(AcScanfAnswer());

      // if Node already exist
      if (AcinatorCheckNode(tree, new_node)) {
        return 1;
      }
      
      AcAddNewNode(tree, new_node, node);
    }

  }

  // It is sign
  else {
    
    if (answer[0] == 'y')
      AcinatorFind(tree, &((*node)->right));
    else
      AcinatorFind(tree, &((*node)->left));
  }

  return 1;
}

///////////////////////////// Helpful functions ///////////////////////////////
char* AcScanfAnswer() {
  static char answer[MAX_LEN_ANSWER] = {};
  
  scanf("%[^\n]", answer);
     
  ClearStdin();
  
  printf("\n");
  return answer;

 }



void AcAddNewNode(Tree* tree, char* new_node, Node** node) {
  assert(new_node);
  assert(node);
  
  // insert new node
  SAY("Nice, And, what is differences between them");
  printf("Nice! And, what is difference between %s?\n", (*node)->data);
  printf("Guys from");
  PRINT_GREEN_E(N(" %s "), new_node);
  PRINT_GREEN(BOLD("HAVE"));
  printf(" this sign, but guys from");
  PRINT_RED_E(N(" %s "), (*node)->data);
  PRINT_RED(BOLD("NOT HAVE"));
  printf(" it.\nType this sign:");
      
  // insert sign for new node
  char* sign = NULL;
  while (1) {
    sign = strdup(AcScanfAnswer());
    
    PRINT_CYAN_E(N("-%s-\n"), sign);
    
    // if user write NOT/NO
    if (strncasecmp("not", sign, 3) == 0) {
        SAY("Too hard for me, please write sign without NO/NOT");
        printf("\nToo hard for me, please write sign without NO/NOT\n");
        printf("Type this sign:");

        continue;
    }

    break;
  } 
      
  SAY("Ok, I will remember this");
  printf("Ok, I will remember this\n");
  printf("------------------------\n\n");
  AcinatorTreePush(tree, node, sign, (*node)->data, new_node);
}












///////////////////////////// Helpful functions ///////////////////////////////
char** AcRotate(char** path, size_t size);

///////////////////////////////////////////////////////////////////////////////
//
//                          Give Difference function
//          Give difference between first_element and second_element
//
////////////////////////////////////////////////////////////////////////////////
int AcinatorGiveDifference(Tree* tree, char* first_element, char* second_element) {
  assert(tree);
  assert(first_element);
  assert(second_element);

  size_t  first_path_size = 0,
         second_path_size = 0;
  
  char** path_to_first  = AcGivePath(tree, first_element,  &first_path_size);
  char** path_to_second = AcGivePath(tree, second_element, &second_path_size);  
  if (path_to_first == NULL || path_to_second == NULL || *path_to_first == NULL || *path_to_second == NULL) {
    SAY("Element do not exist!");
    PRINT_RED(BLINK("Element do not exist!\n"));
    return 0;
  }
  path_to_first  = AcRotate(path_to_first, first_path_size);
  path_to_second = AcRotate(path_to_second, second_path_size);
  
  ///// FIRST ELEMENT /////
  PRINT_MAGENTA(BOLD("First element:  "));

  // Analyse first element
  if (!TreeCompare(path_to_first[0], path_to_second[0]))
      PRINT_GREEN_E(BOLD("%s"), path_to_first[0]);
    else
      PRINT_RED_E(BOLD("%s"), path_to_first[0]);
  
  // Analyse other elements
  for (int i = 1; i < first_path_size; ++i) {
    
    if (i >= second_path_size) {
      PRINT_RED_E(BOLD(", %s"), path_to_first[i]);
      continue;
    }

    if (!TreeCompare(path_to_first[i], path_to_second[i]))
      PRINT_GREEN_E(BOLD(", %s"), path_to_first[i]);
    else
      PRINT_RED_E(BOLD(", %s"), path_to_first[i]);
  }
  printf("\n");


  ///// SECOND ELEMENT /////
  PRINT_MAGENTA(BOLD("Second element: "));

  // Analyse first element
  if (!TreeCompare(path_to_first[0], path_to_second[0]))
      PRINT_GREEN_E(BOLD("%s"), path_to_second[0]);
    else
      PRINT_RED_E(BOLD("%s"), path_to_second[0]);
  
  // Analyse other elements
  for (int i = 1; i < second_path_size; ++i) {
    
    if (i >= first_path_size) {
      PRINT_RED_E(BOLD(", %s"), path_to_second[i]);
      continue;
    }

    if (!TreeCompare(path_to_first[i], path_to_second[i]))
      PRINT_GREEN_E(BOLD(", %s"), path_to_second[i]);
    else
      PRINT_RED_E(BOLD(", %s"), path_to_second[i]);
  }
  
  free(path_to_second);
  free(path_to_first);
  printf("\n");
  return 1;
}

///////////////////////////// Helpful functions ///////////////////////////////
char** AcRotate(char** path, size_t size) {
  assert(path);

  char** new_path = (char**)calloc(size, sizeof(new_path[0]));
  
  // strref
  for (int i = 0; i < size; ++i)
    new_path[i] = path[size - i - 1];
  
  free(path);
  return new_path;
}










int main() {
  Tree tree = {};
  TreeLoad(&tree, "Trees/acinator_tree");  
  //AcinatorTreeInit(&tree, "Love Physics?", "MSU", "FRTK");
  
  GreedFunction();
   
  for (int i = 0; i < 50; ++i) {
    char command[MAX_LEN_ANSWER] = {};
    scanf("%s", command);

    if (strcmp(command, "add") == 0) {
      ClearStdin();
      SAY("I will give you some questions");
      
      AcinatorFind(&tree, &(tree.root));
    }

    else if (strcmp(command, "diff") == 0) {
      ClearStdin();

      char  first[MAX_LEN_ANSWER] = {};
      char second[MAX_LEN_ANSWER] = {};
      
      SAY("Write through whitespace with whom: ");
      printf("Write through whitespace with whom: ");
      
      scanf("%s %s", first, second);
      SAY("Give me some seconds");
      AcinatorGiveDifference(&tree, first, second);
    }

    else if (strcmp(command, "print") == 0) {
      ClearStdin();
      SAY("you are welcom");

      TreePrint(&tree, "DoxyFiles/acinator.doxy");
    }

    else if (strcmp(command, "save") == 0) {
      ClearStdin();
      TreeSave(&tree, "Trees/acinator_tree");
      
      SAY("Tree was saved");
      PRINT_CYAN(N("Tree was saved\n"));
    }

    else if (strcmp(command, "end") == 0) {
      ClearStdin();
      SAY("Goodbye my friend");
      
      system("clear");
      return 0;
    }

    else 
      ClearStdin();

  }
  
  return 0;
}
