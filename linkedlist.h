#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

typedef struct node {
	void* value;
	struct node* next;
} LIST_NODE;

LIST_NODE* createNewNode(void* value) {
	LIST_NODE* node = malloc(sizeof(LIST_NODE));
	node->value = value;
	node->next = NULL;

	return node;
}

void freeNode(LIST_NODE* node) {
	free(node->value);
	free(node);
}

typedef struct list {
	LIST_NODE* header;
	int size;
} LINKED_LIST;


LINKED_LIST* createEmptyList() {
	LINKED_LIST* list = malloc(sizeof(LINKED_LIST));
	list->header = NULL;
	list->size = 0;

	return list;
}

void forEachValue(LINKED_LIST* list, void (*func)(void*)) {
	for (LIST_NODE* currentNode = list->header; currentNode != NULL; currentNode = currentNode->next)
		(*func)(currentNode->value);
}

LIST_NODE* findNodeAt(LINKED_LIST* list, int index) {
	if (index < 0 || index > list->size - 1) return NULL;

	LIST_NODE* currentNode;
	int currentNodeIndex = 0;

	for (currentNode = list->header; currentNode != NULL; currentNode = currentNode->next)
		if (currentNodeIndex++ == index) break;

	return currentNode;
}

void appendToList(LINKED_LIST* list, void* value) {
	//new values are appended at the start of the given list.

	LIST_NODE* currentNode = createNewNode(value);
	LIST_NODE* previousNode = list->header;

	list->header = currentNode;
	currentNode->next = previousNode;

	(list->size)++;
}

bool removeHeaderNode(LINKED_LIST* list) {
	if (list->header == NULL) return false;

	LIST_NODE* header = list->header;
	list->header = header->next;
	freeNode(header);
	(list->size)--;

	return true;
}

bool removeNodeAt(LINKED_LIST* list, int index) {
	if (index < 0 || index > list->size - 1)
		return false;
	else if (index == 0)
		return removeHeaderNode(list);
	else {
		LIST_NODE* previousNode = findNodeAt(list, index - 1);
		LIST_NODE* nodeToRemove = previousNode->next;
		previousNode->next = nodeToRemove->next;
		freeNode(nodeToRemove);
		(list->size)--;

		return true;
	}
}

void clearList(LINKED_LIST* list) {
	while (removeHeaderNode(list));
}

#endif