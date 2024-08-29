// Kitap Maðazasý Otomasyonu

// Yusuf DÝLER - 032290015
// Burak ZÜMBÜL - 032190026
// Sena ÝNANKUL - 032190049
// Mehmet Ali ERDAL - 032290052
// Yavuzhan Semih BAYZAN - 032290067
// Amin NAJAFLÝ - 032290094

// BMB2006

// Dosya ismi: grup7_KitapMaðazasýOtomasyonu


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>

#define RETURNTIME 15

typedef struct {
    int mm, dd, yy;
} Date;

typedef struct {
    int id;
    char stname[20];
    char name[20];
    char author[20];
    int quantity;
    float price;
    int rackno;
    char category[20];
    Date issued;
    Date duedate;
    int available;
} Book;

typedef struct Node {
    Book book;
    struct Node* next;
} Node;
typedef Node* Nodeptr;

typedef struct StackNode {
    Book book;
    struct StackNode* next;
} StackNode;
typedef StackNode* Stackptr;

typedef struct QueueNode {
    Book book;
    struct QueueNode* next;
} QueueNode;
typedef QueueNode* kuyrukptr;

typedef struct TreeNode {
    Book book;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;
typedef TreeNode* agacptr;

typedef struct GraphNode {
    Book book;
    struct GraphNode* next;
    struct GraphEdge* edges;
} GraphNode;
typedef GraphNode* grafptr;

typedef struct GraphEdge {
    grafptr target;
    struct GraphEdge* next;
} GraphEdge;
typedef GraphEdge* grafkenarptr;

typedef struct User {
    char username[20];
    char password[20];
} User;


Nodeptr bookList = NULL;
Stackptr bookStack = NULL;
kuyrukptr front = NULL;
kuyrukptr rear = NULL;
agacptr bookTree = NULL;
grafptr bookGraph = NULL;
User* userList = NULL;
int userCount = 0;

char categories[][15] = {"Computer", "Philosophy", "Law", "Electronic", "Medicine", "Architecture"};

int s;
Book a;

void returnFunc();
void mainMenu();
void addBooks();
void deleteBooks();
void editBooks();
void searchBooks();
void issueBooks();
void viewBooks();
int getdata();
int checkId(int);
void login();
void issueRecord();
void initializeBooks();
void inorderTraversal(agacptr root);
agacptr deleteBookTree(agacptr root, int id);
agacptr findMin(agacptr root);
agacptr updateBookTree(agacptr root, Book updatedBook);

Nodeptr createNode(Book book) {
    Nodeptr newNode = (Nodeptr)malloc(sizeof(Node));
    newNode->book = book;
    newNode->next = NULL;
    return newNode;
}

void insertBook(Nodeptr* head, Book book) {
    Nodeptr newNode = createNode(book);
    newNode->next = *head;
    *head = newNode;
}

void deleteBook(Nodeptr* head, int id) {
    Nodeptr temp = *head;
    Nodeptr prev = NULL;
    while (temp != NULL && temp->book.id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    if (prev == NULL) {
        *head = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp);
}

Stackptr createStackNode(Book book) {
    Stackptr newNode = (Stackptr)malloc(sizeof(StackNode));
    newNode->book = book;
    newNode->next = NULL;
    return newNode;
}

void pushBook(Stackptr* top, Book book) {
    Stackptr newNode = createStackNode(book);
    newNode->next = *top;
    *top = newNode;
}

void popBook(Stackptr* top, int id) {
    Stackptr temp = *top;
    Stackptr prev = NULL;
    while (temp != NULL && temp->book.id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    if (prev == NULL) {
        *top = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp);
}

kuyrukptr createQueueNode(Book book) {
    kuyrukptr newNode = (kuyrukptr)malloc(sizeof(QueueNode));
    newNode->book = book;
    newNode->next = NULL;
    return newNode;
}

void enqueueBook(kuyrukptr* front, kuyrukptr* rear, Book book) {
    kuyrukptr newNode = createQueueNode(book);
    if (*rear == NULL) {
        *front = *rear = newNode;
        return;
    }
    (*rear)->next = newNode;
    *rear = newNode;
}

void dequeueBook(kuyrukptr* front, kuyrukptr* rear, int id) {
    kuyrukptr temp = *front;
    kuyrukptr prev = NULL;
    while (temp != NULL && temp->book.id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    if (prev == NULL) {
        *front = temp->next;
    } else {
        prev->next = temp->next;
    }
    if (*rear == temp) {
        *rear = prev;
    }
    free(temp);
}

agacptr createTreeNode(Book book) {
    agacptr newNode = (agacptr)malloc(sizeof(TreeNode));
    newNode->book = book;
    newNode->left = newNode->right = NULL;
    return newNode;
}

agacptr insertBookTree(agacptr node, Book book) {
    if (node == NULL) return createTreeNode(book);

    if (book.id < node->book.id) {
        node->left = insertBookTree(node->left, book);
    } else if (book.id > node->book.id) {
        node->right = insertBookTree(node->right, book);
    }

    return node;
}

agacptr deleteBookTree(agacptr root, int id) {
    if (root == NULL) return root;

    if (id < root->book.id) {
        root->left = deleteBookTree(root->left, id);
    } else if (id > root->book.id) {
        root->right = deleteBookTree(root->right, id);
    } else {
        if (root->left == NULL) {
            agacptr temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            agacptr temp = root->left;
            free(root);
            return temp;
        }

        agacptr temp = findMin(root->right);
        root->book = temp->book;
        root->right = deleteBookTree(root->right, temp->book.id);
    }
    return root;
}

agacptr findMin(agacptr root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

agacptr updateBookTree(agacptr root, Book updatedBook) {
    if (root == NULL) return NULL;

    if (updatedBook.id < root->book.id) {
        root->left = updateBookTree(root->left, updatedBook);
    } else if (updatedBook.id > root->book.id) {
        root->right = updateBookTree(root->right, updatedBook);
    } else {
        root->book = updatedBook;
    }

    return root;
}

void inorderTraversal(agacptr root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("\n\t%-15s %-10d %-20s %-20s %-10d %-10.2f %-10d %-10s", root->book.category, root->book.id, root->book.name, root->book.author, root->book.quantity, root->book.price, root->book.rackno, root->book.available ? "Available" : root->book.stname);
        inorderTraversal(root->right);
    }
}

grafptr createGraphNode(Book book) {
    grafptr newNode = (grafptr)malloc(sizeof(GraphNode));
    newNode->book = book;
    newNode->next = NULL;
    newNode->edges = NULL;
    return newNode;
}

grafkenarptr createGraphEdge(grafptr target) {
    grafkenarptr newEdge = (grafkenarptr)malloc(sizeof(GraphEdge));
    newEdge->target = target;
    newEdge->next = NULL;
    return newEdge;
}

void addGraphEdge(grafptr source, grafptr target) {
    grafkenarptr newEdge = createGraphEdge(target);
    newEdge->next = source->edges;
    source->edges = newEdge;
}

void insertBookGraph(grafptr* graph, Book book) {
    grafptr newNode = createGraphNode(book);
    newNode->next = *graph;
    *graph = newNode;
}

grafptr findGraphNode(grafptr graph, int id) {
    grafptr temp = graph;
    while (temp != NULL) {
        if (temp->book.id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void connectBooksByCategory(grafptr graph) {
    grafptr current = graph;
    while (current != NULL) {
        grafptr target = graph;
        while (target != NULL) {
            if (target != current && strcmp(current->book.category, target->book.category) == 0) {
                addGraphEdge(current, target);
            }
            target = target->next;
        }
        current = current->next;
    }
}

void printGraph(grafptr graph) {
    grafptr temp = graph;
    while (temp != NULL) {
        printf("\nBook ID: %d, Name: %s, Category: %s\nConnected to: ", temp->book.id, temp->book.name, temp->book.category);
        grafkenarptr edge = temp->edges;
        while (edge != NULL) {
            printf("%d ", edge->target->book.id);
            edge = edge->next;
        }
        printf("\n");
        temp = temp->next;
    }
}

void initializeBooks() {
    int i;
    Book books[50] = {
        {1, "", "Book 1", "Author 1", 5, 100.0, 1, "Computer", {0, 0, 0}, {0, 0, 0}, 1},
        {2, "", "Book 2", "Author 2", 3, 150.0, 2, "Philosophy", {0, 0, 0}, {0, 0, 0}, 1},
        {3, "", "Book 3", "Author 3", 4, 200.0, 3, "Law", {0, 0, 0}, {0, 0, 0}, 1},
        {4, "", "Book 4", "Author 4", 2, 250.0, 4, "Electronic", {0, 0, 0}, {0, 0, 0}, 1},
        {5, "", "Book 5", "Author 5", 6, 300.0, 5, "Medicine", {0, 0, 0}, {0, 0, 0}, 1},
        {6, "", "Book 6", "Author 6", 1, 350.0, 6, "Architecture", {0, 0, 0}, {0, 0, 0}, 1},
        {7, "", "Book 7", "Author 7", 3, 100.0, 1, "Computer", {0, 0, 0}, {0, 0, 0}, 1},
        {8, "", "Book 8", "Author 8", 4, 150.0, 2, "Philosophy", {0, 0, 0}, {0, 0, 0}, 1},
        {9, "", "Book 9", "Author 9", 5, 200.0, 3, "Law", {0, 0, 0}, {0, 0, 0}, 1},
        {10, "", "Book 10", "Author 10", 2, 250.0, 4, "Electronic", {0, 0, 0}, {0, 0, 0}, 1},
        {11, "", "Book 11", "Author 11", 6, 300.0, 5, "Medicine", {0, 0, 0}, {0, 0, 0}, 1},
        {12, "", "Book 12", "Author 12", 7, 350.0, 6, "Architecture", {0, 0, 0}, {0, 0, 0}, 1},
        {13, "", "Book 13", "Author 13", 3, 100.0, 1, "Computer", {0, 0, 0}, {0, 0, 0}, 1},
        {14, "", "Book 14", "Author 14", 4, 150.0, 2, "Philosophy", {0, 0, 0}, {0, 0, 0}, 1},
        {15, "", "Book 15", "Author 15", 5, 200.0, 3, "Law", {0, 0, 0}, {0, 0, 0}, 1},
        {16, "", "Book 16", "Author 16", 2, 250.0, 4, "Electronic", {0, 0, 0}, {0, 0, 0}, 1},
        {17, "", "Book 17", "Author 17", 6, 300.0, 5, "Medicine", {0, 0, 0}, {0, 0, 0}, 1},
        {18, "", "Book 18", "Author 18", 7, 350.0, 6, "Architecture", {0, 0, 0}, {0, 0, 0}, 1},
        {19, "", "Book 19", "Author 19", 3, 100.0, 1, "Computer", {0, 0, 0}, {0, 0, 0}, 1},
        {20, "", "Book 20", "Author 20", 4, 150.0, 2, "Philosophy", {0, 0, 0}, {0, 0, 0}, 1},
        {21, "", "Book 21", "Author 21", 5, 200.0, 3, "Law", {0, 0, 0}, {0, 0, 0}, 1},
        {22, "", "Book 22", "Author 22", 2, 250.0, 4, "Electronic", {0, 0, 0}, {0, 0, 0}, 1},
        {23, "", "Book 23", "Author 23", 6, 300.0, 5, "Medicine", {0, 0, 0}, {0, 0, 0}, 1},
        {24, "", "Book 24", "Author 24", 7, 350.0, 6, "Architecture", {0, 0, 0}, {0, 0, 0}, 1},
        {25, "", "Book 25", "Author 25", 3, 100.0, 1, "Computer", {0, 0, 0}, {0, 0, 0}, 1},
        {26, "", "Book 26", "Author 26", 4, 150.0, 2, "Philosophy", {0, 0, 0}, {0, 0, 0}, 1},
        {27, "", "Book 27", "Author 27", 5, 200.0, 3, "Law", {0, 0, 0}, {0, 0, 0}, 1},
        {28, "", "Book 28", "Author 28", 2, 250.0, 4, "Electronic", {0, 0, 0}, {0, 0, 0}, 1},
        {29, "", "Book 29", "Author 29", 6, 300.0, 5, "Medicine", {0, 0, 0}, {0, 0, 0}, 1},
        {30, "", "Book 30", "Author 30", 7, 350.0, 6, "Architecture", {0, 0, 0}, {0, 0, 0}, 1},
        {31, "", "Book 31", "Author 31", 3, 100.0, 1, "Computer", {0, 0, 0}, {0, 0, 0}, 1},
        {32, "", "Book 32", "Author 32", 4, 150.0, 2, "Philosophy", {0, 0, 0}, {0, 0, 0}, 1},
        {33, "", "Book 33", "Author 33", 5, 200.0, 3, "Law", {0, 0, 0}, {0, 0, 0}, 1},
        {34, "", "Book 34", "Author 34", 2, 250.0, 4, "Electronic", {0, 0, 0}, {0, 0, 0}, 1},
        {35, "", "Book 35", "Author 35", 6, 300.0, 5, "Medicine", {0, 0, 0}, {0, 0, 0}, 1},
        {36, "", "Book 36", "Author 36", 7, 350.0, 6, "Architecture", {0, 0, 0}, {0, 0, 0}, 1},
        {37, "", "Book 37", "Author 37", 3, 100.0, 1, "Computer", {0, 0, 0}, {0, 0, 0}, 1},
        {38, "", "Book 38", "Author 38", 4, 150.0, 2, "Philosophy", {0, 0, 0}, {0, 0, 0}, 1},
        {39, "", "Book 39", "Author 39", 5, 200.0, 3, "Law", {0, 0, 0}, {0, 0, 0}, 1},
        {40, "", "Book 40", "Author 40", 2, 250.0, 4, "Electronic", {0, 0, 0}, {0, 0, 0}, 1},
        {41, "", "Book 41", "Author 41", 6, 300.0, 5, "Medicine", {0, 0, 0}, {0, 0, 0}, 1},
        {42, "", "Book 42", "Author 42", 7, 350.0, 6, "Architecture", {0, 0, 0}, {0, 0, 0}, 1},
        {43, "", "Book 43", "Author 43", 3, 100.0, 1, "Computer", {0, 0, 0}, {0, 0, 0}, 1},
        {44, "", "Book 44", "Author 44", 4, 150.0, 2, "Philosophy", {0, 0, 0}, {0, 0, 0}, 1},
        {45, "", "Book 45", "Author 45", 5, 200.0, 3, "Law", {0, 0, 0}, {0, 0, 0}, 1},
        {46, "", "Book 46", "Author 46", 2, 250.0, 4, "Electronic", {0, 0, 0}, {0, 0, 0}, 1},
        {47, "", "Book 47", "Author 47", 6, 300.0, 5, "Medicine", {0, 0, 0}, {0, 0, 0}, 1},
        {48, "", "Book 48", "Author 48", 7, 350.0, 6, "Architecture", {0, 0, 0}, {0, 0, 0}, 1},
        {49, "", "Book 49", "Author 49", 3, 100.0, 1, "Computer", {0, 0, 0}, {0, 0, 0}, 1},
        {50, "", "Book 50", "Author 50", 4, 150.0, 2, "Philosophy", {0, 0, 0}, {0, 0, 0}, 1}
    };

    for (i = 0; i < 50; i++) {
        insertBook(&bookList, books[i]);
        pushBook(&bookStack, books[i]);
        enqueueBook(&front, &rear, books[i]);
        bookTree = insertBookTree(bookTree, books[i]);
        insertBookGraph(&bookGraph, books[i]);
    }

    connectBooksByCategory(bookGraph);

    userList = (User*)malloc(2 * sizeof(User));
    strcpy(userList[0].username, "admin");
    strcpy(userList[0].password, "1234");
    strcpy(userList[1].username, "user");
    strcpy(userList[1].password, "5678");
    userCount = 2;
}

void login() {
    char username[20];
    char pass[20];
    int i = 0, j;
    char ch;

    system("cls");
    printf("\n\t");
    for(int i=0;i<52;i++){
        usleep(8000);
        printf("-");
    }
    printf("\n\t*                                                  *");
    printf("\n\t*                   LOGIN                          *");
    printf("\n\t*                                                  *\n\t");
    for(int i=0;i<52;i++){
        usleep(8000);
        printf("-");
    }

    printf("\n\tEnter Username: ");
    scanf("%s", username);
    printf("\n\tEnter Password: ");
    i = 0;
    while ((ch = getch()) != 13) {
        pass[i++] = ch;
        printf("*");
    }
    pass[i] = '\0';

    for (j = 0; j < userCount; j++) {
        if (strcmp(username, userList[j].username) == 0 && strcmp(pass, userList[j].password) == 0) {
            printf("\n\n\tLogin Successful!");
            printf("\n\tPress any key to continue...");
            getch();
            mainMenu();
            return;
        }
    }

    printf("\n\n\tLogin Failed!");
    printf("\n\tPress any key to try again...");
    getch();
    login();
}

void mainMenu() {
    system("cls");
    printf("\n\t");
    for(int i=0;i<52;i++){
        usleep(5000);
        printf("-");
    }
    printf("\n\t*                                                  *");
    printf("\n\t*                   MAIN MENU                      *");
    printf("\n\t*                                                  *");
    printf("\n\t----------------------------------------------------\n");
    printf("\t1. Add Books\n");
    printf("\t2. Delete Books\n");
    printf("\t3. Search Books\n");
    printf("\t4. Issue Books\n");
    printf("\t5. View Book List\n");
    printf("\t6. Edit Book's Record\n");
    printf("\t7. Close Application\n");
    printf("\tPlease enter your choice: ");
    scanf("%d", &s);

    switch (s) {
        case 1:
            addBooks();
            break;
        case 2:
            deleteBooks();
            break;
        case 3:
            searchBooks();
            break;
        case 4:
            issueBooks();
            break;
        case 5:
            viewBooks();
            break;
        case 6:
            editBooks();
            break;
        case 7:
            exit(0);
        default:
            printf("\tInvalid entry. Please enter a valid choice.\n");
            mainMenu();
    }
}

void addBooks() {
    system("cls");
    printf("\n\t");
    for(int i=0;i<52;i++){
        usleep(5000);
        printf("-");
    }
    printf("\n\t*                   ADD BOOKS                      *");
    printf("\n\t*                                                  *");
    printf("\n\t----------------------------------------------------\n");
    printf("Enter the information of the book:\n");

    printf("ID: ");
    scanf("%d", &a.id);
    printf("Name: ");
    scanf("%s", a.name);
    printf("Author: ");
    scanf("%s", a.author);
    printf("Quantity: ");
    scanf("%d", &a.quantity);
    printf("Price: ");
    scanf("%f", &a.price);
    printf("Rack No: ");
    scanf("%d", &a.rackno);
    printf("Category: ");
    scanf("%s", a.category);
    a.available = 1;

    insertBook(&bookList, a);
    pushBook(&bookStack, a);
    enqueueBook(&front, &rear, a);
    bookTree = insertBookTree(bookTree, a);
    insertBookGraph(&bookGraph, a);
    connectBooksByCategory(bookGraph);

    printf("The record is successfully saved.\n");
    printf("Save any more? (Y / N): ");
    if (getch() == 'n') {
        mainMenu();
    } else {
        addBooks();
    }
}

void deleteBooks() {
    system("cls");
    printf("\n\t----------------------------------------------------");
    printf("\n\t*                                                  *");
    printf("\n\t*                 DELETE BOOKS                     *");
    printf("\n\t*                                                  *");
    printf("\n\t----------------------------------------------------\n");

    int id;
    printf("Enter the Book ID to delete: ");
    scanf("%d", &id);

    bookTree = deleteBookTree(bookTree, id);
    deleteBook(&bookList, id);
    popBook(&bookStack, id);
    dequeueBook(&front, &rear, id);

    printf("The record is successfully deleted.\n");
    printf("Delete another record? (Y / N): ");
    if (getch() == 'n') {
        mainMenu();
    } else {
        deleteBooks();
    }
}

void editBooks() {
    system("cls");
    printf("\n\t----------------------------------------------------");
    printf("\n\t*                                                  *");
    printf("\n\t*                 EDIT BOOKS                       *");
    printf("\n\t*                                                  *");
    printf("\n\t----------------------------------------------------\n");

    int id;
    printf("Enter the Book ID to edit: ");
    scanf("%d", &id);

    agacptr bookNode = bookTree;
    while (bookNode != NULL && bookNode->book.id != id) {
        if (id < bookNode->book.id) {
            bookNode = bookNode->left;
        } else {
            bookNode = bookNode->right;
        }
    }

    if (bookNode == NULL) {
        printf("No record found.\n");
        returnFunc();
        return;
    }

    printf("Current Book Information:\n");
    printf("ID: %d\n", bookNode->book.id);
    printf("Name: %s\n", bookNode->book.name);
    printf("Author: %s\n", bookNode->book.author);
    printf("Quantity: %d\n", bookNode->book.quantity);
    printf("Price: %.2f\n", bookNode->book.price);
    printf("Rack No: %d\n", bookNode->book.rackno);
    printf("Category: %s\n", bookNode->book.category);

    printf("Enter the new information of the book:\n");
    printf("Name: ");
    scanf("%s", bookNode->book.name);
    printf("Author: ");
    scanf("%s", bookNode->book.author);
    printf("Quantity: ");
    scanf("%d", &bookNode->book.quantity);
    printf("Price: ");
    scanf("%f", &bookNode->book.price);
    printf("Rack No: ");
    scanf("%d", &bookNode->book.rackno);
    printf("Category: ");
    scanf("%s", bookNode->book.category);

    bookTree = updateBookTree(bookTree, bookNode->book);

    printf("The record is successfully updated.\n");
    printf("Update another record? (Y / N): ");
    if (getch() == 'n') {
        mainMenu();
    } else {
        editBooks();
    }
}

void searchBooks() {
    system("cls");
    printf("\n\t----------------------------------------------------");
    printf("\n\t*                                                  *");
    printf("\n\t*                 SEARCH BOOKS                     *");
    printf("\n\t*                                                  *");
    printf("\n\t----------------------------------------------------\n");

    int id;
    printf("Enter the Book ID to search: ");
    scanf("%d", &id);

    agacptr bookNode = bookTree;
    while (bookNode != NULL && bookNode->book.id != id) {
        if (id < bookNode->book.id) {
            bookNode = bookNode->left;
        } else {
            bookNode = bookNode->right;
        }
    }

    if (bookNode == NULL) {
        printf("No record found.\n");
        returnFunc();
        return;
    }

    printf("Book Information:\n");
    printf("ID: %d\n", bookNode->book.id);
    printf("Name: %s\n", bookNode->book.name);
    printf("Author: %s\n", bookNode->book.author);
    printf("Quantity: %d\n", bookNode->book.quantity);
    printf("Price: %.2f\n", bookNode->book.price);
    printf("Rack No: %d\n", bookNode->book.rackno);
    printf("Category: %s\n", bookNode->book.category);
    printf("Availability: %s\n", bookNode->book.available ? "Available" : bookNode->book.stname);

    printf("Search another book? (Y / N): ");
    if (getch() == 'n') {
        mainMenu();
    } else {
        searchBooks();
    }
}

void viewBooks() {
    system("cls");
    printf("\n\t----------------------------------------------------");
    printf("\n\t*                                                  *");
    printf("\n\t*                  BOOK LIST                       *");
    printf("\n\t*                                                  *");
    printf("\n\t----------------------------------------------------\n");
    printf("\n\t%-15s %-10s %-20s %-20s %-10s %-10s %-10s %-10s", "CATEGORY", "ID", "BOOK NAME", "AUTHOR", "QUANTITY", "PRICE", "RACK NO", "STATUS");
    inorderTraversal(bookTree);
    printf("\n\n\tTotal Books: ");
    returnFunc();
}

void issueBooks() {
    system("cls");
    printf("\n\t----------------------------------------------------");
    printf("\n\t*                                                  *");
    printf("\n\t*                 ISSUE BOOKS                      *");
    printf("\n\t*                                                  *");
    printf("\n\t----------------------------------------------------\n");
    printf("\n\t1. Issue a book");
    printf("\n\t2. View issued books");
    printf("\n\t3. Search issued books");
    printf("\n\t4. Remove issued books");
    printf("\n\tEnter your choice: ");
    switch (getch()) {
        case '1':
            system("cls");
            printf("\n\t----------------------------------------------------");
            printf("\n\t*                                                  *");
            printf("\n\t*                 ISSUE A BOOK                     *");
            printf("\n\t*                                                  *");
            printf("\n\t----------------------------------------------------\n");
            printf("\n\tEnter Book ID: ");
            int id;
            scanf("%d", &id);
            Nodeptr temp = bookList;
            while (temp != NULL) {
                if (temp->book.id == id) {
                    printf("\n\tBook available");
                    printf("\n\tBook name: %s", temp->book.name);
                    printf("\n\tEnter student name: ");
                    scanf("%s", temp->book.stname);
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    temp->book.issued.dd = tm.tm_mday;
                    temp->book.issued.mm = tm.tm_mon + 1;
                    temp->book.issued.yy = tm.tm_year + 1900;
                    temp->book.duedate.dd = temp->book.issued.dd + RETURNTIME;
                    temp->book.duedate.mm = temp->book.issued.mm;
                    temp->book.duedate.yy = temp->book.issued.yy;
                    if (temp->book.duedate.dd > 30) {
                        temp->book.duedate.mm += temp->book.duedate.dd / 30;
                        temp->book.duedate.dd %= 30;
                    }
                    if (temp->book.duedate.mm > 12) {
                        temp->book.duedate.yy += temp->book.duedate.mm / 12;
                        temp->book.duedate.mm %= 12;
                    }
                    temp->book.available = 0;
                    printf("\n\tIssued date: %d-%d-%d", temp->book.issued.dd, temp->book.issued.mm, temp->book.issued.yy);
                    printf("\n\tDue date: %d-%d-%d", temp->book.duedate.dd, temp->book.duedate.mm, temp->book.duedate.yy);
                    printf("\n\tBook issued successfully");
                    break;
                }
                temp = temp->next;
            }
            if (temp == NULL) {
                printf("\n\tNo record found");
            }
            printf("\n\tIssue another book? (Y/N)");
            if (getch() == 'n') mainMenu();
            else issueBooks();
            break;
        case '2':
            system("cls");
            printf("\n\t----------------------------------------------------");
            printf("\n\t*                                                  *");
            printf("\n\t*                ISSUED BOOK LIST                  *");
            printf("\n\t*                                                  *");
            printf("\n\t----------------------------------------------------\n");
            temp = bookList;
            while (temp != NULL) {
                if (!temp->book.available) {
                    printf("\n\t%s\t%s\t%d\t%s\t%d-%d-%d\t%d-%d-%d", temp->book.stname, temp->book.category, temp->book.id, temp->book.name, temp->book.issued.dd, temp->book.issued.mm, temp->book.issued.yy, temp->book.duedate.dd, temp->book.duedate.mm, temp->book.duedate.yy);
                }
                temp = temp->next;
            }
            getch();
            mainMenu();
            break;
        case '3':
            system("cls");
            printf("\n\t----------------------------------------------------");
            printf("\n\t*                                                  *");
            printf("\n\t*                SEARCH ISSUED BOOKS               *");
            printf("\n\t*                                                  *");
            printf("\n\t----------------------------------------------------\n");
            printf("\n\tEnter Book ID to search: ");
            scanf("%d", &id);
            temp = bookList;
            while (temp != NULL) {
                if (temp->book.id == id && !temp->book.available) {
                    issueRecord();
                    printf("\n\tPress any key to continue...");
                    getch();
                    break;
                }
                temp = temp->next;
            }
            if (temp == NULL) {
                printf("\n\tNo record found");
            }
            printf("\n\tSearch another book? (Y/N)");
            if (getch() == 'y') issueBooks();
            else mainMenu();
            break;
        case '4':
            system("cls");
            printf("\n\t----------------------------------------------------");
            printf("\n\t*                                                  *");
            printf("\n\t*                 REMOVE ISSUED BOOKS              *");
            printf("\n\t*                                                  *");
            printf("\n\t----------------------------------------------------\n");
            printf("\n\tEnter Book ID to remove: ");
            scanf("%d", &id);
            temp = bookList;
            while (temp != NULL) {
                if (temp->book.id == id && !temp->book.available) {
                    issueRecord();
                    printf("\n\tRemove the book? (Y/N)");
                    if (getch() == 'y') {
                        temp->book.available = 1;
                        strcpy(temp->book.stname, "");
                        printf("\n\tBook removed successfully");
                    }
                    break;
                }
                temp = temp->next;
            }
            if (temp == NULL) {
                printf("\n\tNo record found");
            }
            printf("\n\tRemove another book? (Y/N)");
            if (getch() == 'y') issueBooks();
            else mainMenu();
            break;
        default:
            printf("\n\tInvalid choice!");
            getch();
            issueBooks();
    }
}

void returnFunc() {
    printf("\n\tPress ENTER to return to Main Menu");
    while (getch() != 13);
    mainMenu();
}

int getdata() {
    int id;
    printf("\n\tEnter the Information Below");
    printf("\n\tCategory: %s", categories[s - 1]);
    printf("\n\tBook ID: ");
    scanf("%d", &id);
    if (checkId(id) == 0) {
        printf("\n\tBook ID already exists");
        getch();
        mainMenu();
        return 0;
    }
    a.id = id;
    printf("\n\tBook Name: ");
    scanf("%s", a.name);
    printf("\n\tAuthor: ");
    scanf("%s", a.author);
    printf("\n\tQuantity: ");
    scanf("%d", &a.quantity);
    printf("\n\tPrice: ");
    scanf("%f", &a.price);
    printf("\n\tRack No: ");
    scanf("%d", &a.rackno);
    return 1;
}

int checkId(int id) {
    Nodeptr temp = bookList;
    while (temp != NULL) {
        if (temp->book.id == id) {
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

void issueRecord() {
    system("cls");
    printf("\n\tBook issued to: %s", a.stname);
    printf("\n\tIssued Date: %d-%d-%d", a.issued.dd, a.issued.mm, a.issued.yy);
    printf("\n\tDue Date: %d-%d-%d", a.duedate.dd, a.duedate.mm, a.duedate.yy);
}

int main() {
    initializeBooks();
    login();
    return 0;
}
