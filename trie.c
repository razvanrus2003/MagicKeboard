/*
	SD 2023 - Trie
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "trie.h"

trie_node_t *trie_create_node(trie_t *trie)
{
	trie_node_t *node = malloc(sizeof(trie_node_t));
	node->n_children = 0;
	node->end_of_word = 0;
	node->value = calloc(1, sizeof(trie->data_size));
	node->children = calloc(trie->alphabet_size, sizeof(trie_node_t));
	return node;
}

trie_t *trie_create(int data_size, int alphabet_size, char *alphabet,
					void (*free_value_cb)(void *))
{
	trie_t *trie = (trie_t *)malloc(sizeof(trie_t));
	trie->size = 0;
	trie->data_size = data_size;
	trie->alphabet = alphabet;
	trie->alphabet_size = alphabet_size;
	trie->n = 1;
	trie->free_value_cb = free_value_cb;
	trie->root = trie_create_node(trie);
	*(int *)(trie->root->value) = -1;
	return trie;
}

int ret_pos(char *alphabet, char letter)
{
	for (size_t i = 0; i < strlen(alphabet); i++) {
		if (letter == alphabet[i])
			return i;
	}
	return -1;
}

void __trie_insert(trie_node_t *node, char *key, void *value, trie_t *trie)
{
	if (key[0] == '\0') {
		node->end_of_word = 1;
		memcpy(node->value, value, trie->data_size);
	} else {
		int pos = ret_pos(trie->alphabet, key[0]);

		if (node->children[pos]) {
			__trie_insert(node->children[pos], key + 1, value, trie);
		} else {
			node->children[pos] = trie_create_node(trie);
			node->n_children += 1;
			trie->n += 1;
			__trie_insert(node->children[pos], key + 1, value, trie);
		}
	}
}

void trie_insert(trie_t *trie, char *key, void *value)
{
	if (!key || !trie || !key[0])
		return;

	int pos = ret_pos(trie->alphabet, key[0]);
	if (trie->root->children[pos]) {
		__trie_insert(trie->root->children[pos], key + 1, value, trie);
	} else {
		trie->root->children[pos] = trie_create_node(trie);
		trie->root->n_children += 1;
		trie->n += 1;
		__trie_insert(trie->root->children[pos], key + 1, value, trie);
	}
	trie->size += 1;
}

void *trie_search(trie_t *trie, char *key)
{
	trie_node_t *node = trie->root;
	if (!key[0])
		return trie->root->value;

	int pos;
	for (size_t i = 0; i < strlen(key); i++) {
		pos = ret_pos(trie->alphabet, key[i]);
		if (!node->children[pos])
			return NULL;
		node = node->children[pos];
	}
	if (node->end_of_word)
		return node->value;
	return NULL;
}

int __trie_remove(trie_node_t *node, char *key, trie_t *trie)
{
	if (key[0] == '\0') {
		node->end_of_word = 0;
		if (!node->n_children) {
			trie->free_value_cb(node->value);
			free(node->children);
			free(node);
			trie->n -= 1;
			return 1;
		}
		return 0;
	}
	int pos = ret_pos(trie->alphabet, key[0]);
	if (__trie_remove(node->children[pos], key + 1, trie)) {
		if (node->n_children == 1 && !node->end_of_word) {
			trie->free_value_cb(node->value);
			free(node->children);
			free(node);
			trie->n -= 1;
			return 1;
		}
		node->n_children -= 1;
		node->children[pos] = NULL;
	}
	return 0;
}

void trie_remove(trie_t *trie, char *key)
{
	if (!key || !trie || !key[0])
		return;
	if (trie_search(trie, key)) {
		int pos = ret_pos(trie->alphabet, key[0]);

		if (__trie_remove(trie->root->children[pos], key + 1, trie)) {
			trie->root->n_children -= 1;
			trie->root->children[pos] = NULL;
		}
		trie->size -= 1;
	}
}

void __trie_free(trie_node_t *node, trie_t *trie)
{
	if (!node->n_children) {
		trie->free_value_cb(node->value);
		free(node->children);
		free(node);
		return;
	}

	for (int i = 0; i < trie->alphabet_size; i++)
		if (node->children[i])
			__trie_free(node->children[i], trie);

	trie->free_value_cb(node->value);
	free(node->children);
	free(node);
}

void trie_free(trie_t **ptrie)
{
	for (int i = 0; i < (*ptrie)->alphabet_size; i++)
		if ((*ptrie)->root->children[i])
			__trie_free((*ptrie)->root->children[i], (*ptrie));

	(*ptrie)->free_value_cb((*ptrie)->root->value);
	free((*ptrie)->root->children);
	free((*ptrie)->root);
	free(*ptrie);
	*ptrie = NULL;
}
