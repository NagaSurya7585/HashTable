#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Define a structure for key-value pairs
struct KeyValue {
    char* key;
    char* value;
    struct KeyValue* next;
};


// Define the hash table structure
struct Hash_table {
    struct KeyValue* table[TABLE_SIZE];  //Two structures are defined: KeyValue to represent key-value pairs, and Hash_table to represent the hash table itself. Hash_table contains an array of KeyValue pointers and an integer size to track the number of elements in the table.
    int size;
};



int hash(const char* key) {
    int index = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        index += key[i];
    }
    return index % TABLE_SIZE;
}
// The hash function calculates a hash index for a given key. It adds up the ASCII values of characters in the key and returns the result modulo TABLE_SIZE to ensure the index is within the valid range.

struct KeyValue* createKeyValue(const char* key, const char* value) {
    struct KeyValue* KY_value = (struct KeyValue*)malloc(sizeof(struct KeyValue));
    if (KY_value) {
        KY_value->key = strdup(key);
        KY_value->value = strdup(value);
        KY_value->next = NULL;
    }
    return KY_value;
}

  // The createKeyValue function allocates memory for a new key-value pair, copies the key and value strings, and initializes the next pointer to NULL. It returns a pointer to the new KeyValue structure.

void inserting_Key(struct Hash_table* h_table, const char* key, const char* value) {
    int index = hash(key);
    struct KeyValue* KY_value = createKeyValue(key, value);

    if (KY_value) {
        KY_value->next = h_table->table[index];
        h_table->table[index] = KY_value;
        h_table->size++;
    }
}

// The inserting_Key function inserts a key-value pair into the hash table. It calculates the index using the hash function, creates a new KeyValue structure, links it to the existing list of entries at that index (if any), and updates the hash table's size.

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

// The get function getting a value associated with a key from the hash table. It calculates the index, iterates through the linked list at that index, and returns the value when the key is found. If the key is not found, it returns "Key not found."

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

// The removing_Key function removes a key-value pair from the hash table. It calculates the index, iterates through the linked list at that index, finds the key to be removed, updates the linked list to skip the removed entry, and frees the memory associated with the removed entry.

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

// The resize function resizes the hash table when the load factor exceeds a threshold. It creates a new table with double the size of the original, rehashes all existing key-value pairs into the new table, and updates the hash table's size and table pointers.


int main() {

        // Initialize the hash table
    struct Hash_table h_table;
    h_table.size = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        h_table.table[i] = NULL;
    }

      // Insert key-value pairs
    inserting_Key(&h_table, "name", "Surya");
    inserting_Key(&h_table, "age", "24");
    inserting_Key(&h_table, "city", "Bangalore");
    
     // Insert key-value pairs
    printf("Name: %s\n", get(&h_table, "name"));
    printf("Age: %s\n", get(&h_table, "age"));
    printf("City: %s\n", get(&h_table, "city"));
    

     // Remove a key-value pair
    removing_Key(&h_table, "age");

    printf("Name: %s\n", get(&h_table, "name"));
    printf("Age: %s\n", get(&h_table, "age"));


 // Check if resizing is needed and perform resizing
    if (h_table.size >= TABLE_SIZE * 0.7) {
        resize(&h_table);
    }

     // Free memory for key-value pairs (cleanup)
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

   // In the main function, the hash table is initialized, key-value pairs are inserted, values are retrieved and printed, a key-value pair is removed, and resizing is performed if the load factor exceeds a threshold. Finally, memory for key-value pairs is freed during cleanup.
}
