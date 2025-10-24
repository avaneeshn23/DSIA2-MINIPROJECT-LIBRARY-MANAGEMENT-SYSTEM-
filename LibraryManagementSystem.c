#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------------------------------
// Structures
// -------------------------------

// Book node (Linked List)
struct Book {
    int id;
    char title[100];
    char author[100];
    int available; // 1 = available, 0 = issued
    struct Book *next;
};

// Request node (Queue)
struct Request {
    int bookId;
    int userId;
    struct Request *next;
};

// Returned book node (Stack)
struct ReturnedBook {
    int bookId;
    struct ReturnedBook *next;
};

// -------------------------------
// Global heads
// -------------------------------
struct Book *bookHead = NULL;          // Linked list of books
struct Request *front = NULL, *rear = NULL; // Queue for issue requests
struct ReturnedBook *top = NULL;       // Stack for returned books

// -------------------------------
// Function declarations
// -------------------------------
void addBook();
void displayBooks();
void issueBook();
void returnBook();
void processReturns();
void viewRequests();

void enqueue(int bookId, int userId);
void dequeue();
void push(int bookId);
int pop();
struct Book* findBookById(int id);

// -------------------------------
// Utility Functions
// -------------------------------

// Add a new book to the linked list
void addBook() {
    struct Book *newBook = (struct Book*)malloc(sizeof(struct Book));
    printf("\nEnter Book ID: ");
    scanf("%d", &newBook->id);
    getchar(); // clear newline
    printf("Enter Book Title: ");
    fgets(newBook->title, 100, stdin);
    newBook->title[strcspn(newBook->title, "\n")] = '\0';
    printf("Enter Author Name: ");
    fgets(newBook->author, 100, stdin);
    newBook->author[strcspn(newBook->author, "\n")] = '\0';
    newBook->available = 1;
    newBook->next = NULL;

    // Insert at end of linked list
    if (bookHead == NULL)
        bookHead = newBook;
    else {
        struct Book *temp = bookHead;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newBook;
    }
    printf("✅ Book added successfully!\n");
}

// Display all books
void displayBooks() {
    if (bookHead == NULL) {
        printf("\n📚 No books in library.\n");
        return;
    }
    printf("\n------ Library Books ------\n");
    struct Book *temp = bookHead;
    while (temp != NULL) {
        printf("ID: %d | Title: %s | Author: %s | Status: %s\n",
               temp->id, temp->title, temp->author,
               temp->available ? "Available" : "Issued");
        temp = temp->next;
    }
}

// Find book by ID
struct Book* findBookById(int id) {
    struct Book *temp = bookHead;
    while (temp != NULL) {
        if (temp->id == id)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

// Enqueue request (Queue)
void enqueue(int bookId, int userId) {
    struct Request *newReq = (struct Request*)malloc(sizeof(struct Request));
    newReq->bookId = bookId;
    newReq->userId = userId;
    newReq->next = NULL;
    if (rear == NULL) {
        front = rear = newReq;
    } else {
        rear->next = newReq;
        rear = newReq;
    }
}

// Dequeue request (Queue)
void dequeue() {
    if (front == NULL)
        return;
    struct Request *temp = front;
    front = front->next;
    if (front == NULL)
        rear = NULL;
    free(temp);
}

// Push onto stack (Returned Books)
void push(int bookId) {
    struct ReturnedBook *newNode = (struct ReturnedBook*)malloc(sizeof(struct ReturnedBook));
    newNode->bookId = bookId;
    newNode->next = top;
    top = newNode;
}

// Pop from stack (Returned Books)
int pop() {
    if (top == NULL)
        return -1;
    int id = top->bookId;
    struct ReturnedBook *temp = top;
    top = top->next;
    free(temp);
    return id;
}

// -------------------------------
// Main Operations
// -------------------------------

// Issue book to a user
void issueBook() {
    int id, userId;
    printf("\nEnter Book ID to issue: ");
    scanf("%d", &id);
    printf("Enter User ID: ");
    scanf("%d", &userId);

    struct Book *book = findBookById(id);
    if (book == NULL) {
    printf("❌ Book not found.\n");
        return;
    }
    if (book->available) {
        book->available = 0;
        printf("✅ Book issued successfully to User %d!\n", userId);
    } else {
        printf("⚠️ Book currently unavailable. Added to waiting queue.\n");
        enqueue(id, userId);
    }
}

// Return a book (push to stack)
void returnBook() {
    int id;
    printf("\nEnter Book ID to return: ");
    scanf("%d", &id);
    push(id);
    printf("✅ Book return recorded! (Awaiting processing)\n");
}

// Process returned books (pop from stack)
void processReturns() {
    if (top == NULL) {
        printf("\n📦 No returned books to process.\n");
        return;
    }
    printf("\nProcessing returned books...\n");
    while (top != NULL) {
        int id = pop();
        struct Book *book = findBookById(id);
        if (book != NULL) {
            book->available = 1;
            printf("✅ Book '%s' (ID %d) marked as available.\n", book->title, id);
            // Check if someone is waiting for it
            if (front != NULL && front->bookId == id) {
                printf("📨 Issuing book to waiting user %d.\n", front->userId);
                book->available = 0;
                dequeue();
            }
        } else {
            printf("⚠️ Book with ID %d not found in library.\n", id);
        }
    }
}

// View pending requests
void viewRequests() {
    if (front == NULL) {
        printf("\n📭 No pending requests.\n");
        return;
    }
    printf("\n------ Pending Requests ------\n");
    struct Request *temp = front;
    while (temp != NULL) {
        printf("Book ID: %d | User ID: %d\n", temp->bookId, temp->userId);
        temp = temp->next;
    }
}

// -------------------------------
// MAIN MENU
// -------------------------------
int main() {
    int choice;
    printf("=====================================\n");
    printf("📚 LIBRARY MANAGEMENT SYSTEM (C)\n");
    printf("=====================================\n");

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Process Returns\n");
        printf("6. View Pending Requests\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: processReturns(); break;
            case 6: viewRequests(); break;
            case 7:
                printf("👋 Exiting system. Goodbye!\n");
                exit(0);
            default:
                printf("❌ Invalid choice. Try again.\n");
        }
    }
    return 0;
}
