#include <stdio.h>
#include <string.h>
#include "trie.h"

void auto3(trie_node_t *node, trie_t *trie, char *buff,
		   int *freq, char *frequent)
{
	if (*(int *)node->value > *freq) {
		*freq = *(int *)node->value;
		strcpy(frequent, buff);
	}
	int i, j = strlen(buff);
	for (i = 0; i < trie->alphabet_size; i++) {
		if (node->children[i]) {
			buff[j] = 'a' + i;
			auto3(node->children[i], trie, buff, freq, frequent);
		}
	}
	buff[j] = '\0';
}

void auto2(trie_node_t *node, trie_t *trie, char *buff, char *shortest)
{
	if (node->end_of_word) {
		strcpy(shortest, buff);
		return;
	}
	int i, j = strlen(buff);
	for (i = 0; i < trie->alphabet_size; i++) {
		if (node->children[i] && strlen(buff) < strlen(shortest) - 1) {
			buff[j] = 'a' + i;
			auto2(node->children[i], trie, buff, shortest);
			buff[j] = '\0';
		}
	}
}

void auto1(trie_node_t *node, trie_t *trie, char *buff)
{
	if (node->end_of_word)
		return;
	int i, j = strlen(buff);
	for (i = 0; i < trie->alphabet_size; i++)
		if (node->children[i]) {
			buff[j] = 'a' + i;
			break;
		}
	auto1(node->children[i], trie, buff);
}

void autocomplete(trie_node_t *node, char *key, trie_t *trie, char *buff,
				  int mod)
{
	if (!key[0]) {
		int j = strlen(buff);
		auto1(node, trie, buff);

		if (mod == 1 || mod == 0)
			printf("%s\n", buff);

		char *shortest = calloc(100, sizeof(char));
		char *frequent = calloc(100, sizeof(char));
		int *freq = malloc(sizeof(int));
		*freq = 0;

		strcpy(shortest, buff);

		int n = strlen(buff);
		for (; j < n; j++)
			buff[j] = '\0';

		if (mod == 2 || mod == 0) {
			auto2(node, trie, buff, shortest);
			printf("%s\n", shortest);
		}

		if (mod == 3 || mod == 0) {
			auto3(node, trie, buff, freq, frequent);
			printf("%s\n", frequent);
		}
		free(freq);
		free(frequent);
		free(shortest);
		return;
	}
	int pos = ret_pos(trie->alphabet, key[0]), j = strlen(buff);
	if (node->children[pos]) {
		buff[j] = 'a' + pos;
		autocomplete(node->children[pos], key + 1, trie, buff, mod);
		buff[j] = '\0';
	} else {
		printf("No words found\n");
		if (mod == 0) {
			printf("No words found\n");
			printf("No words found\n");
		}
	}
}

void autocorrect(trie_node_t *node, char *key, trie_t *trie, int k,
				 char *buff, int *found)
{
	if (!key[0] && node->end_of_word) {
		printf("%s\n", buff);
		*found = 1;
		return;
	}
	if (!key[0])
		return;
	int pos = ret_pos(trie->alphabet, key[0]), i, j = strlen(buff);
	for (i = 0; i < trie->alphabet_size; i++) {
		if (i == pos && node->children[pos]) {
			buff[j] = 'a' + i;
			autocorrect(node->children[pos], key + 1, trie, k, buff, found);
		} else if (node->children[i] && k) {
			buff[j] = 'a' + i;
			autocorrect(node->children[i], key + 1, trie, k - 1, buff, found);
		}
	}
	buff[j] = '\0';
}

int main(void)
{
	trie_t *trie = trie_create(sizeof(int), ALPHABET_SIZE, ALPHABET, free);
	char *s = (char *)malloc(sizeof(char) * 100), *token, *buff, *key;
	int *value, *init_value = malloc(sizeof(int)), pos, i, j, k, found = 0;
	*init_value = 1;
	buff = (char *)calloc(100, sizeof(char));
	scanf("%[^\n]%*c", s);
	while (strcmp(s, "EXIT")) {
		token = strtok(s, " ");
		if (!strcmp(token, "INSERT")) {
			token = strtok(NULL, " ");
			value = trie_search(trie, token);
			if (value)
				*value += 1;
			else
				trie_insert(trie, token, init_value);
		}
		if (!strcmp(token, "LOAD")) {
			token = strtok(NULL, " ");
			FILE *f = fopen(token, "rt");
			char *word = (char *)malloc(100 * sizeof(char));
			while (!feof(f)) {
				fscanf(f, "%s", word);
				value = trie_search(trie, word);
				if (value)
					*value += 1;
				else
					trie_insert(trie, word, init_value);
			}
			free(word);
			fclose(f);
		}
		if (!strcmp(token, "AUTOCORRECT")) {
			token = strtok(NULL, " ");
			pos = ret_pos(trie->alphabet, token[0]);
			j = 0;
			key = token;
			token = strtok(NULL, " ");
			k = atoi(token);
			found = 0;
			for (i = 0; i < trie->alphabet_size; i++) {
				if (i == pos && trie->root->children[pos]) {
					buff[j] = 'a' + i;
					autocorrect(trie->root->children[pos], key + 1, trie, k,
								buff, &found);
				} else if (trie->root->children[i] && k) {
					buff[j] = 'a' + i;
					autocorrect(trie->root->children[i], key + 1, trie, k - 1,
								buff, &found);
				}
			}
			if (!found)
				printf("No words found\n");
			buff[j] = '\0';
		}
		if (!strcmp(token, "AUTOCOMPLETE")) {
			token = strtok(NULL, " ");
			pos = ret_pos(trie->alphabet, token[0]);
			j = 0;
			key = token;
			token = strtok(NULL, " ");
			k = token[0] - '0';
			if (trie->root->children[pos]) {
				buff[j] = 'a' + pos;
				autocomplete(trie->root->children[pos], key + 1, trie, buff, k);
			}
		}
		if (!strcmp(token, "REMOVE")) {
			token = strtok(NULL, " ");
			trie_remove(trie, token);
		}
		scanf("%[^\n]%*c", s);
	}
	free(buff);
	free(init_value);
	trie_free(&trie);
	free(s);
	return 0;
}
