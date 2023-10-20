/*
	SD 2023 - Trie
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
	/* Value associated with key (set if end_of_word = 1) */
	void *value;

	/* 1 if current node marks the end of a word, 0 otherwise */
	int end_of_word;

	trie_node_t **children;
	int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
	trie_node_t *root;

	/* Number of keys */
	int size;

	/* Generic Data Structure */
	int data_size;

	/* Trie-Specific, alphabet properties */
	int alphabet_size;
	char *alphabet;

	/* Callback to free value associated with key, called when freeing */
	void (*free_value_cb)(void *data);

	/* Optional - number of nodes, useful to test correctness */
	int n;
};

trie_node_t *trie_create_node(trie_t *trie);

trie_t *trie_create(int data_size, int alphabet_size, char *alphabet,
					void (*free_value_cb)(void *));

int ret_pos(char *alphabet, char letter);

void __trie_insert(trie_node_t *node, char *key, void *value, trie_t *trie);

void trie_insert(trie_t *trie, char *key, void *value);

void *trie_search(trie_t *trie, char *key);

int __trie_remove(trie_node_t *node, char *key, trie_t *trie);

void trie_remove(trie_t *trie, char *key);

void __trie_free(trie_node_t *node, trie_t *trie);

void trie_free(trie_t **ptrie);
