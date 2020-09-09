int TreeSave(Tree* tree, const char* file_name, int mode) {
  assert(tree);

  FILE* output = NULL;
  if (OpenFile(file_name, "w", &output))
    return 1;
  
  switch (mode) {
    
    case PREORDER:
      PreorderSaveNodes(tree->root, output);
      break;
    
    case INORDER:
      InorderSaveNodes(tree->root, output);
      break;


    case POSTORDER:
      PostorderSaveNodes(tree->root, output);
      break;

    default:
      return 1;
    
  }

  fclose(output);
  return 0;
}

void PreorderSaveNodes(Node* node, FILE* file) {
  assert(file);
  
  if (node == NULL) {
    fprintf(file, "nil");
    return;
  }

     fprintf(file, "{");
  
  fprintf(file, "\"%" TELEM_SYMBOL "\"", node->data);
  PreorderSaveNodes(node->right, file);
  PreorderSaveNodes(node->left, file);
  
    fprintf(file, "}");
}

void PostorderSaveNodes(Node* node, FILE* file) {
  assert(file);
  
  if (node == NULL) {
    fprintf(file, "nil");
    return;
  }

     fprintf(file, "{");
  
  PostorderSaveNodes(node->left, file);
  PostorderSaveNodes(node->right, file);
  fprintf(file, "\"%" TELEM_SYMBOL "\"", node->data);
  
    fprintf(file, "}");
}

void InorderSaveNodes(Node* node, FILE* file) {
  assert(file);
  
  if (node == NULL) {
    fprintf(file, "nil");
    return;
  }

     fprintf(file, "{");
  
  InorderSaveNodes(node->left, file);
  fprintf(file, "\"%" TELEM_SYMBOL "\"", node->data);
  InorderSaveNodes(node->right, file);
  
    fprintf(file, "}");
}
