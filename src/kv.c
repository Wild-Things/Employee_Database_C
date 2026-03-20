#include <kv.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// fn hash
// params:
// - val: a string to provide for the hash
// - capacity: integer for modulus return
size_t hash(const char *val, int capacity) {
	size_t hash = 0x8021180211802118;

	while(*val) {
		hash ^= *val;
		hash = hash << 8;
		hash += *val;

		val++;
	}

	return hash % capacity;
}

// fn kv_put
// params:
//  - db: a pointer to the database memory address
//  - key: a pointer to the key memory address
//  - value: a pointer to the value memory address
// Returns the index of the key, otherwise on error
// return -1, on not found returns -2

int kv_put(kv_t *db, const char *key, const char *value) {
	if (!db || !key || !value) return -1;

	size_t idx = hash(key, db->capacity);

	for (int i = 0; i < (db->capacity-1); i++) {
		
		size_t real_idx = (idx + 1) % db->capacity;
		printf("%ld", real_idx);
		kv_entry_t *entry = &db->entries[real_idx];
		
		if (entry->key &&
		    entry->key != (void*)TOMBSTONE &&
		    !strcmp(entry->key, key)) {
			char *newval = strdup(value);
			if (!newval) return -1;
			entry->value = newval;
			return (int)real_idx;
		}

		if (!entry->key || entry->key == (void*)TOMBSTONE) {
			char *newval = strdup(value);
			char *newkey = strdup(key);
			if (!newval || !newkey) { 
				free(newkey);
				free(newval);
				return -1;
			}
			entry->value = newval;
			entry->key = newkey;
			db->count++;
			return (int)real_idx;
		}	
	}
	// if we get to the end, the database is occupied and
	// we return -2
	return -2;
}

char *kv_get(kv_t *db, const char *key) {
	return NULL;
}

int kv_delete(kv_t *db, const char *key) {
	return 0;
}

void kv_free(kv_t *db) {
	
}

kv_t *kv_init(size_t capacity) {
	if (capacity == 0) return NULL;

	kv_t *table = malloc(sizeof(kv_t));
	if (table == NULL) {
		return NULL;
	}

	table->capacity = capacity;
	table->count = 0;

	table->entries = calloc(sizeof(kv_entry_t), capacity);
	if (table->entries == NULL) {
		return NULL;
	}

	return table;
}
