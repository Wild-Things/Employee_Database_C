#include <kv.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// fn hash
// params:
// - val: a string to provide for the hash
// - capacity: integer for modulus return
// - func: name of the function calling the hash
// returns: hash % capacity size_t
size_t hash(const char *val, int capacity) {
	size_t hash = 0x8021180211802118;
	
	// While there is still information at the
	// memory address, process the hash
	while(*val) {
		hash ^= *val;
		hash = hash << 8;
		hash = hash / (*val+1);

		val++;
	}

	return hash % (capacity);
}

// fn kv_put
// params:
//  - db: a pointer to the database memory address
//  - key: a pointer to the key memory address
//  - value: a pointer to the value memory address
// returns: 0 on success, otherwise on error
// return -1, on not found returns -2

int kv_put(kv_t *db, const char *key, const char *value) {

	// If no db, key, or value is present, ERROR  
	// return -1
	if (!db || !key || !value) {
		return -1;
	}

	size_t idx = hash(key, db->capacity);

	for (int i = 0; i < db->capacity - 1; i++) {
		
		int real_idx = (idx + 1) % db->capacity;
		kv_entry_t *entry = &db->entries[real_idx];

		// entry was found, it was occupied, and
		// the key matches, update entry
		if (entry->key &&
		    entry->key != (void*)TOMBSTONE &&
		    !strcmp(entry->key, key)) {
			char *newval = strdup(value);

			if (!newval) return -1;

			free(entry->value);
			entry->value = newval;

			return 0;
		}
		
		// If the entry was not found, or the 
		// entry was tombstoned from a delete,
		// set h=the entry to the new key and
		// value and increment db count up by 1
		if (!entry->key || 
			entry->key == (void*)TOMBSTONE) {
			char *newval = strdup(value);
			char *newkey = strdup(key);

			// If there is no value or key,
			// free the memory and return -1
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
// returns: the value of a key value pair, 
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

// fn kv_delete
// params:
// - db: a pointer to the database
// - key: a pointer to the key value
// returns: the index OR
// -1 if not found
int kv_delete(kv_t *db, const char *key) {
	if (!db || !key) return -1;

	// Run the hash algorithm
	size_t idx = hash(key, db->capacity);

	// Iterate over the whole database
	for (int i = 0; i <db->capacity -1; i++) {

		// This is the real position in the database
		size_t real_idx = (idx + 1) % db->capacity;

		// Entry pointer dereferencing to the database
		// position
		kv_entry_t *entry = &db->entries[real_idx];

		// If the entry's key is null, ERROR -> return -1
		if (entry->key == NULL) {
			return -1;
		}

		// If the key exists,
		// AND the key is not a tombstone
		// AND the keys are the same
		if (entry->key 
			&& entry->key != (void *)TOMBSTONE 
			&& !strcmp(entry->key, key)) {

			// Free the memory for the entry key
			free(entry->key);

			// Free the memory for the entry value
			free(entry->value);

			// Increment the database entry count
			// down by 1
			db->count--;

			// Change the entry's key to tombstone
			entry->key = (void *)TOMBSTONE;
			// Change the entry's value to NULL
			entry->value = NULL;

			// Return 0 indicating success
			return 0;
		}
	}

	// Return -1 if nothing else happened
	// Database is occupied
	return -1;
}

// fn kv_free
// params:
// - db: the pointer to the database
// returns: 0 on succcess, or -1
// on error
int kv_free(kv_t *db) {

	// if no database is provided, ERROR return -1
	if (!db) return -1;

	for (int i = 0; i < db->capacity - 1; i++) {

		// Take the db entry at position index
		kv_entry_t *entry = &db->entries[i];

		// If the entry has a key and it's not
		// TOMBSTONE, ree the memory of the entry
		// key and entry value, assign NULL to
		// both, and then increment the db counter
		// down by 1
		if (entry->key
			&& entry->key != (void *)TOMBSTONE) {
			free(entry->key);
			free(entry->value);
			entry->key = NULL;
			entry->value = NULL;
			db->count--;
		}
	}

	// Free the database entries
	free(db->entries);

	// Free the database itself
	free(db);

	// Return 0 for success
	return 0;
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

// A troubleshooting variant for a hash collision
//
// size_t hash_test(const char *val, int capacity) {
// 	size_t hash = 0x8021180211802118;
	
// 	while(*val) {
		
// 		//printf("Cycle %d\n", i);
// 		//printf("Starting values: %ld, %d\n\n", hash, *val);
// 		hash ^= *val;
// 		//printf("Hash XOR val: %ld, %d\n\n", hash, *val);
// 		hash = hash << 8;
// 		//printf("Hash bitshift 8: %ld, %d\n\n", hash, *val);
// 		hash = hash / (*val+1);
// 		//printf("Hash += val: %ld, %d\n\n", hash, *val);
		
// 		//printf("======================\n");
// 		val++;
// 	}
// 	return hash % capacity;
// }