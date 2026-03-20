#include <stdio.h>
#include <kv.h>

int main() {
	kv_t *table = kv_init(16);
	printf("%p\n", table);

	printf("capacity = %ld\n", table->capacity);

	kv_put(table, "fruit", "apple");
	kv_put(table, "veggie", "carrot");
	kv_put(table, "grain", "wheat");
	kv_put(table, "protein", "egg");

	char *val1 = kv_get(table, "fruit");
	char *val2 = kv_get(table, "veggie");
	char *val3 = kv_get(table, "grain");
	char *val4 = kv_get(table, "protein");

	for (int i = 0; i < table->capacity; i++) {
		if (table->entries[i].key) {
			printf("[%d] %s: %s\n",
			i,	
			table->entries[i].key, 
			table->entries[i].value);
		}
	}
	printf("\n");
	printf("%s\n%s\n%s\n%s\n\n", val1, val2, val3, val4);

	int fruit = hash_test("fruit", 16);
	int veggie = hash_test("veggie", 16);
	int grain = hash_test("grain", 16);
	int protein = hash_test("protein", 16);

	printf("Fruit: %d\n\n", fruit);
	printf("Veggie: %d\n\n", veggie);
	printf("Grain: %d\n\n", grain);
	printf("Protein: %d\n\n", protein);
}
