#include <stdio.h>
#include <kv.h>

int main() {

	//Initializes the database
	kv_t *table = kv_init(16);

	// Prints the memory address of the table
	printf("%p\n", table);

	// Prints the table capacity to the terminal
	printf("capacity = %ld\n", table->capacity);

	// New line for formatting
	printf("\n");

	// Inserts entries into the database table	
	kv_put(table, "fruit", "apple");
	kv_put(table, "veggie", "carrot");
	kv_put(table, "grain", "wheat");
	kv_put(table, "protein", "egg");

	// Retrieves the value of the entry that contains
	// the respective key and assigns it to a
	// variable
	char *val1 = kv_get(table, "fruit");
	char *val2 = kv_get(table, "veggie");
	char *val3 = kv_get(table, "grain");
	char *val4 = kv_get(table, "protein");

	// New line for formatting
	printf("\n");

	// Iteration for all database entries
	// Prints all entries
	for (int i = 0; i < table->capacity; i++) {
		if (table->entries[i].key) {
			printf("[%d] %s: %s\n",
			i,	
			table->entries[i].key, 
			table->entries[i].value);
		}
	}

	// New line for formatting
	printf("\n");

	// Test output for kv_get
	printf("%s\n%s\n%s\n%s\n\n", val1, val2, val3, val4);

	kv_delete(table, "fruit");

	// int fruit = hash_test("fruit", 16);
	// int veggie = hash_test("veggie", 16);
	// int grain = hash_test("grain", 16);
	// int protein = hash_test("protein", 16);

	// printf("Fruit: %d\n\n", fruit);
	// printf("Veggie: %d\n\n", veggie);
	// printf("Grain: %d\n\n", grain);
	// printf("Protein: %d\n\n", protein);
}
