#include <stdio.h>
#include <stdlib.h>

struct trie {
    struct trie **arr_pbranches;
    void *value;
    int cnt;
};

void trie_clear(struct trie *ptrie) {
    ptrie->arr_pbranches = NULL;
    ptrie->value = NULL;
    ptrie->cnt = 0;
}

void add_word(
    struct trie *ptrie,
    char *word
) {
    int i = 0;
    char c;
    while (c = word[i]) {
        if (!ptrie->arr_pbranches) {
            ptrie->arr_pbranches = malloc(256 * sizeof(struct trie *));
        }
        struct trie *pbranch = ptrie->arr_pbranches[c];
        if (!pbranch) {
            printf("make branch for %c\n", c);
            pbranch = malloc(sizeof(*pbranch));
            trie_clear(pbranch);
            ptrie->arr_pbranches[c] = pbranch;
        }
        ptrie = pbranch;
        ++i;
    }
    ptrie->value = word;
}

void *get_value(
    struct trie *ptrie,
    char *word
) {
    int i = 0;
    char c;
    while (c = word[i]) {
        if (!ptrie->arr_pbranches) return NULL;
        struct trie *pbranch = ptrie->arr_pbranches[c];
        if (!pbranch) return NULL;
        ptrie = pbranch;
        ++i;
    }
    return ptrie->value;
}

void test() {
    struct trie t;
    trie_clear(&t);
    add_word(&t, "a");
    add_word(&t, "a");
    add_word(&t, "b1");
    add_word(&t, "b2");
    add_word(&t, "b13");
    printf("%s\n", (char *) get_value(&t, "b2"));
    printf("%s\n", (char *) get_value(&t, "b"));
}

int main(int argc, char **argv) {
    test();
    return 0;
}