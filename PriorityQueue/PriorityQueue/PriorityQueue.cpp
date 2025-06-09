//Вариант 1
//Невозрастающая очередь с приоритетами
//Операции: Enqueue, Dequeue, Peek, IncreasePriority, Merge
//Реализация : биномиальная пирамида

//Все операции по времени O(logn)

#include <iostream>

using namespace std;

struct Node {
    int key;
    Node* parent;
    Node* child;
    Node* sibling;
    int degree;

    Node(int k) {
        key = k;
        parent = nullptr;
        child = nullptr;
        sibling = nullptr;
        degree = 0;
    }
};

struct BinomialHeap {
    Node* head;
};

//для основного мерджа
BinomialHeap mergeHeaps(BinomialHeap& h1, BinomialHeap& h2) {
    BinomialHeap newHeap;
    newHeap.head = nullptr;
    Node* carry = nullptr;
    Node* cur1 = h1.head;
    Node* cur2 = h2.head;

    while (cur1 || cur2 || carry) {
        Node* node1 = cur1;
        Node* node2 = cur2;

        if (node1 && node2 && node1->degree == node2->degree) {
            cur1 = cur1->sibling;
            cur2 = cur2->sibling;

            if (node1->key >= node2->key) {
  
                node2->parent = node1;
                node2->sibling = node1->child;
                node1->child = node2;
                node1->degree++;

                node1->sibling = carry;
                carry = node1;
            }
            else {

                node1->parent = node2;
                node1->sibling = node2->child;
                node2->child = node1;
                node2->degree++;

                node2->sibling = carry;
                carry = node2;
            }
            continue;
        }

        Node* toAdd = nullptr;
        int currentDegree = 0;

        if (carry) {
            currentDegree = carry->degree;
            if (node1 && node1->degree == currentDegree) {
                toAdd = node1;
                cur1 = cur1->sibling;
            }
            else if (node2 && node2->degree == currentDegree) {
                toAdd = node2;
                cur2 = cur2->sibling;
            }
            else {
                toAdd = nullptr;
            }

            if (toAdd) {
                if (toAdd->key >= carry->key) {

                    carry->parent = toAdd;
                    carry->sibling = toAdd->child;
                    toAdd->child = carry;
                    toAdd->degree++;
                    carry = toAdd;
                }
                else {

                    toAdd->parent = carry;
                    toAdd->sibling = carry->child;
                    carry->child = toAdd;
                    carry->degree++;
                }
                continue;
            }
            else {
                toAdd = carry;
                carry = nullptr;
            }
        }

        else if (node1 && (!node2 || node1->degree <= node2->degree)) {
            toAdd = node1;
            cur1 = cur1->sibling;
        }
        else if (node2) {
            toAdd = node2;
            cur2 = cur2->sibling;
        }

        if (toAdd) {
            toAdd->sibling = newHeap.head;
            newHeap.head = toAdd;
        }
    }

    Node* prev = nullptr;
    Node* curr = newHeap.head;
    Node* next = nullptr;
    while (curr) {
        next = curr->sibling;
        curr->sibling = prev;
        prev = curr;
        curr = next;
    }
    newHeap.head = prev;

    return newHeap;
}

void enqueue(BinomialHeap& heap, int key) {
    BinomialHeap temp;
    temp.head = new Node(key);
    BinomialHeap newHeap = mergeHeaps(heap, temp);
    heap.head = newHeap.head;
}

int peek(BinomialHeap& heap) {
    if (!heap.head) return -1;

    int maxKey = heap.head->key;
    Node* cur = heap.head->sibling;

    while (cur) {
        if (cur->key > maxKey)
            maxKey = cur->key;
        cur = cur->sibling;
    }

    return maxKey;
}

void dequeue(BinomialHeap& heap) {
    if (!heap.head) return;

    int maxKey = peek(heap);
    Node* prev = nullptr;
    Node* curr = heap.head;

    while (curr && curr->key != maxKey) {
        prev = curr;
        curr = curr->sibling;
    }

    if (!curr) return; 

    if (prev)
        prev->sibling = curr->sibling;
    else
        heap.head = curr->sibling;

    BinomialHeap childrenHeap;
    childrenHeap.head = nullptr;
    Node* child = curr->child;
    while (child) {
        Node* next = child->sibling;
        child->parent = nullptr;
        child->sibling = childrenHeap.head;
        childrenHeap.head = child;
        child = next;
    }

    BinomialHeap newHeap = mergeHeaps(heap, childrenHeap);
    heap.head = newHeap.head;
    delete curr;
}

Node* findNode(Node* node, int key) {
    if (!node) return nullptr;
    if (node->key == key) return node;

    Node* found = findNode(node->child, key);
    if (found) return found;

    return findNode(node->sibling, key);
}

bool increasePriority(BinomialHeap& heap, int oldKey, int newKey) {
    if (newKey < oldKey) {
        cout << "Новый ключ должен быть больше текущего." << endl;
        return false;
    }

    Node* node = findNode(heap.head, oldKey);
    if (!node) {
        cout << "Узел с таким ключом не найден." << endl;
        return false;
    }

    node->key = newKey;
    Node* parent = node->parent;

    while (parent && node->key > parent->key) {
        swap(node->key, parent->key);
        node = parent;
        parent = node->parent;
    }

    return true;
}

void merge(BinomialHeap& h1, BinomialHeap& h2) {
    BinomialHeap newHeap = mergeHeaps(h1, h2);
    h1.head = newHeap.head;
    h2.head = nullptr;
}

int main() {
    BinomialHeap heap1;
    heap1.head = nullptr;

    enqueue(heap1, 5);
    enqueue(heap1, 8);
    enqueue(heap1, 3);
    enqueue(heap1, 2);
    enqueue(heap1, 1);
    enqueue(heap1, 10);

    cout << "Peek - " << peek(heap1) << endl;

    dequeue(heap1);
    cout << "Peek - " << peek(heap1) << endl;

    increasePriority(heap1, 8, 12);
    cout << "Peek - " << peek(heap1) << endl;

    dequeue(heap1);
    cout << "Peek - " << peek(heap1) << endl;

    BinomialHeap heap2;
    heap2.head = nullptr;
    enqueue(heap2, 7);
    enqueue(heap2, 9);

    merge(heap1, heap2);
    cout << "Peek merge: - " << peek(heap1) << endl;
    dequeue(heap1);

    BinomialHeap heap3;
    heap3.head = nullptr;
    enqueue(heap3, 15);
    enqueue(heap3, 6);

    merge(heap1, heap3);
    cout << "Peek - " << peek(heap1) << endl;
    dequeue(heap1);
    cout << "Peek - " << peek(heap1) << endl;
    dequeue(heap1);
    cout << "Peek - " << peek(heap1) << endl;
    dequeue(heap1);
    cout << "Peek - " << peek(heap1) << endl;

    return 0;
}