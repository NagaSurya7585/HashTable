#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

struct KeyValue {
    char* key;
    char* value;
    struct KeyValue* next;
};

struct Hash_table {
    struct KeyValue* table[TABLE_SIZE];
    int size;
};

int hash(const char* key) {
    int index = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        index += key[i];
    }
    return index % TABLE_SIZE;
}

struct KeyValue* createKeyValue(const char* key, const char* value) {
    struct KeyValue* KY_value = (struct KeyValue*)malloc(sizeof(struct KeyValue));
    if (KY_value) {
        KY_value->key = strdup(key);
        KY_value->value = strdup(value);
        KY_value->next = NULL;
    }
    return KY_value;
}

void inserting_Key(struct Hash_table* h_table, const char* key, const char* value) {
    int index = hash(key);
    struct KeyValue* KY_value = createKeyValue(key, value);

    if (KY_value) {
        KY_value->next = h_table->table[index];
        h_table->table[index] = KY_value;
        h_table->size++;
    }
}

const char* get(const struct Hash_table* h_table, const char* key) {
    int index = hash(key);
    struct KeyValue* KY_value = h_table->table[index];

    while (KY_value != NULL) {
        if (strcmp(KY_value->key, key) == 0) {
            return KY_value->value;
        }
        KY_value = KY_value->next;
    }

    return "Key not found";
}

void removing_Key(struct Hash_table* h_table, const char* key) {
    int index = hash(key);
    struct KeyValue* current = h_table->table[index];
    struct KeyValue* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                h_table->table[index] = current->next;
            }
            free(current->key);
            free(current->value);
            free(current);
            h_table->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}
void resize(struct Hash_table* h_table) {
    struct KeyValue* newTable[TABLE_SIZE * 2];
    for (int i = 0; i < TABLE_SIZE * 2; i++) {
        newTable[i] = NULL;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        struct KeyValue* KY_value = h_table->table[i];
        while (KY_value != NULL) {
            struct KeyValue* next = KY_value->next;
            int newIndex = hash(KY_value->key);
            KY_value->next = newTable[newIndex];
            newTable[newIndex] = KY_value;
            KY_value = next;
        }
    }
    h_table->size = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        h_table->table[i] = newTable[i];
    }
}


int main() {
    struct Hash_table h_table;
    h_table.size = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        h_table.table[i] = NULL;
    }

    inserting_Key(&h_table, "name", "Surya");
    inserting_Key(&h_table, "age", "24");
    inserting_Key(&h_table, "city", "Bangalore");

    printf("Name: %s\n", get(&h_table, "name"));
    printf("Age: %s\n", get(&h_table, "age"));
    printf("City: %s\n", get(&h_table, "city"));
    
    removing_Key(&h_table, "age");
    printf("Name: %s\n", get(&h_table, "name"));
    printf("Age: %s\n", get(&h_table, "age"));

    if (h_table.size >= TABLE_SIZE * 0.7) {
        resize(&h_table);
    }
     for (int i = 0; i < TABLE_SIZE; i++) {
        struct KeyValue* KY_value = h_table.table[i];
        while (KY_value != NULL) {
            struct KeyValue* next = KY_value->next;
            free(KY_value->key);
            free(KY_value->value);
            free(KY_value);
            KY_value = next;
        }
    }
    
    return 0;
}
