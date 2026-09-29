typedef struct Node{
    struct Node *left;
    struct Node *right;
    int value;
} Node; 


Node *insert_value(Node *root, int value);

Node *remove_value(Node *root, int value);

Node *search_value(Node *root, int value);