#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

typedef struct ListNode  Node;
typedef struct ListNode* List;
#define nil ((void*) 0)

Node* clone(Node* node) {
    Node* new = malloc(sizeof(*node));
    new->val  = node->val;
    new->next = nil;
    return new;
}

List mergeKLists(List* lists, int listsSize) {
    int list_count = listsSize; 

    List mega_root = nil;
    List mega_list = nil; // oh yeah, this could just be a Node* node in the for loop, like at the end, oops...

    for(;;) {

        Node* min = nil;
        int   min_list_index = 0;
        for(int i = 0; i < list_count; i ++) {
            if(lists[i] == nil) continue;

            if(min == nil || lists[i]->val < min->val) {
                min = lists[i];
                min_list_index = i;
            }
        }

        if(min == nil) break;

        if(mega_root == nil) {
            mega_root = clone(min);
            mega_list = mega_root;
            mega_list->next = malloc(sizeof(Node));
        } else {
            mega_list->next = clone(min);
            mega_list = mega_list->next;
        }

        lists[min_list_index] = min->next;
    }

    for(Node* node = mega_root; node != nil; node = node->next) {
        printf("%d, ", node->val);
    }
    
    return mega_root;
}

Node* l(int value, Node* next) {
    Node* node = malloc(sizeof(*node));
    node->val  = value;
    node->next = next;
    return node;
} 

int main() {
    List a = l(1, l(4, l(5, nil)));
    List b = l(1, l(3, l(4, nil)));
    List c = l(2, l(6, nil));
    
    List A[] = { a, b, c };
    mergeKLists(A, 3);
    

}
