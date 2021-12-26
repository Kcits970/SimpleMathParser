#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#define MAX_LIST_SIZE 100

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

void deleteNode(LIST_NODE* node, void (*del)(void*)) {
	(*del)(node->value);
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

void printAllValues(LINKED_LIST* list, const char* prefix, void (*printFunction)(void*)) {
	int index = 0;

	for (LIST_NODE* currentNode = list->header; currentNode != NULL; currentNode = currentNode->next) {
		printf("%s %-3.3d: ", prefix, ++index);
		(*printFunction)(currentNode->value);
	}
}

LIST_NODE* getNodeAt(LINKED_LIST* list, int index) {
	if (index < 0 || index > list->size - 1) return NULL;

	LIST_NODE* currentNode;
	int currentNodeIndex = 0;

	for (currentNode = list->header; currentNode != NULL; currentNode = currentNode->next)
		if (currentNodeIndex++ == index) break;

	return currentNode;
}

void* getValueAt(LINKED_LIST* list, int index) {
	LIST_NODE* node = getNodeAt(list, index);

	if (node != NULL)
		return node->value;
	else
		return NULL;
}

bool removeHeaderNode(LINKED_LIST* list, void (*del)(void*)) {
	if (list->header == NULL) return false;

	LIST_NODE* header = list->header;
	list->header = header->next;
	deleteNode(header, del);
	(list->size)--;

	return true;
}

bool removeNodeAt(LINKED_LIST* list, int index, void (*del)(void*)) {
	if (index < 0 || index > list->size - 1)
		return false;
	else if (index == 0)
		return removeHeaderNode(list, del);
	else {
		LIST_NODE* previousNode = getNodeAt(list, index - 1);
		LIST_NODE* nodeToRemove = previousNode->next;
		previousNode->next = nodeToRemove->next;
		deleteNode(nodeToRemove, del);
		(list->size)--;

		return true;
	}
}

void clearList(LINKED_LIST* list, void (*del)(void*)) {
	while (removeHeaderNode(list, del));
}

void appendToList(LINKED_LIST* list, void* value, void(*del)(void*)) {
	//new values are appended at the start of the given list.
	//function pointer del is needed in case the list exceeds the specified max size.
	if (list->size == MAX_LIST_SIZE)
		removeNodeAt(list, list->size - 1, del);

	LIST_NODE* currentNode = createNewNode(value);
	LIST_NODE* previousNode = list->header;

	list->header = currentNode;
	currentNode->next = previousNode;

	(list->size)++;
}

#endif