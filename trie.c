#include <stdio.h>
#include <stdlib.h>

/*
char hash:
    void *get(hash *hash, char c)
    void *add(hash *hash, char c, void *value)
    void *data

node:
    void *value
    char_hash ch

empty_hash:
    get = return null
    add = create data, update get
    data = null

ab:
    a:
        get is null,
        create node:
            value = null
            ch = empty hash
        call add a on hash with node
        hash = node.ch

    b:
        get is null
        create node:
            value = null
            ch = empty hash
        call add a on hash with node
        hash = node.ch

    \0:
        set node.value to str_value
*/


struct char_hash {
    void *(*get)(struct char_hash *, char);
    void (*put)(struct char_hash *, char, void *);
    void *data;
};

struct char_entry {
    char c;
    void *value;
};

struct simple_hash {
    int cnt;
    struct char_entry *array;
};

void *char_hash_simple_get(
    struct char_hash *pch,
    char c
) {
    struct simple_hash *psh = pch->data;
    int i;

    for (i = 0; i < psh->cnt; ++i) {
        if (psh->array[i].c == c) {
            return psh->array[i].value;
        }
    }
    return NULL;
}

void char_hash_simple_put(
    struct char_hash *pch,
    char c,
    void *value
) {
    struct simple_hash *psh = pch->data;

    psh->array[psh->cnt].c = c;
    psh->array[psh->cnt].value = value;
    ++psh->cnt;
}

void *char_hash_null_get(
    struct char_hash *pch,
    char c
) {
    return NULL;
}

void char_hash_null_put(
    struct char_hash *pch,
    char c,
    void *value
) {
    pch->get = char_hash_simple_get;
    pch->put = char_hash_simple_put;
    struct simple_hash *psh = malloc(sizeof(*psh));
    pch->data = psh;
    psh->array = malloc(10 * sizeof(struct char_entry));
    char_hash_simple_put(pch, c, value);
}

void char_hash_init(
    struct char_hash *pch
) {
    pch->get = char_hash_null_get;
    pch->put = char_hash_null_put;
}
    
void test_char_hash(
) {
    struct char_hash ch;

    char_hash_init(&ch);
    (ch.put)(&ch, 'a', "alpha");
    (ch.put)(&ch, 'b', "beta");
    printf("%s\n", (char *)(ch.get)(&ch, 'a'));
    printf("%s\n", (char *)(ch.get)(&ch, 'b'));
}

struct trie {
    struct char_hash ch;
    void *value;
};

void trie_clear(struct trie *ptrie) {
    char_hash_init(&ptrie->ch);
    ptrie->value = NULL;
}

void add_word(
    struct trie *ptrie,
    char *word
) {
    int i = 0;
    char c;
    while (c = word[i]) {
        struct char_hash *pch = &(ptrie->ch);
        struct trie *pbranch = (ptrie->ch.get)(pch, c);
        if (!pbranch) {
            pbranch = malloc(sizeof(*pbranch));
            trie_clear(pbranch);
            (ptrie->ch.put)(&(ptrie->ch), c, pbranch);
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
        struct char_hash *pch = &(ptrie->ch);
        struct trie *pbranch = (ptrie->ch.get)(pch, c);
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
    printf("%s\n", (char *) get_value(&t, "b1"));
    printf("%s\n", (char *) get_value(&t, "b13"));
    printf("%s\n", (char *) get_value(&t, "b"));
    printf("%s\n", (char *) get_value(&t, ""));
}

int main(int argc, char **argv) {
    test();
    return 0;
}
