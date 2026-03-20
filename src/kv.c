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

	printf("%ld\n", (hash % (capacity-1)));
	return hash % (capacity-1);
}

// fn kv_put
// params:
//  - db: a pointer to the database memory address
//  - key: a pointer to the key memory address
//  - value: a pointer to the value memory address
// Returns the index of the key, otherwise on error
// return -1, on not found returns -2

int kv_put(kv_t *db, const char *key, const char *value) {

	if (!db || !key || !value) {
		return -1;
	}

	size_t idx = hash(key, db->capacity);	

	for (int i = 0; i < db->capacity-1; i++) {
		
		int real_idx = (idx + 1) % db->capacity;
		kv_entry_t *entry = &db->entries[real_idx];

		// entry was found, it was occupied, and
		// the key matches	
		if (entry->key &&
		    entry->key != (void*)TOMBSTONE &&
		    !strcmp(entry->key, key)) {
			char *newval = strdup(value);
			if (!newval) return -1;
			entry->value = newval;
			return 0;
		}
		
		// Entry was not found, or entry was 
		// tombstoned from a delete
		if (!entry->key || 
			entry->key == (void*)TOMBSTONE) {
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
			return 0;
		}	
	}
	// if we get to the end, the database is occupied and
	// we return -2
	return -2;
}

// fn kv_get
// params:
//  - db: a pointer to the database memory address
//  - key: a pointer to the key memory address
// Returns the value of a key value pair, 
// otherwise return null on error
char *kv_get(kv_t *db, const char *key) {

	// if there are missing arguments,
	// return null
	if (!db || !key) {
		return NULL;
	}

	// hash a key
	size_t idx = hash(key, db->capacity);

	for (int i = 0; i < db->capacity-1; i++) {
		
		int real_idx = (idx + 1) % db->capacity;

		kv_entry_t *entry = &db->entries[real_idx];
		
		// if the entry has no key, or has a 
		// TOMBSTONE value, return nothing
		if (entry->key == NULL || 
			entry->key == (void*)TOMBSTONE) {
			return NULL;
		}

		// if the entry is found and the keys
		// match, return the value
		if (entry->key && 
			entry->key != (void*)TOMBSTONE &&
			!strcmp(entry->key, key)) {
			return entry->value;
		}
	}
	// loop ends without anything found
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
