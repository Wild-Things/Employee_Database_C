#include <stdio.h>
#include <kv.h>

int main() {
	kv_t *table = kv_init(32);
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

	printf("%s, %s, %s, %s\n", val1, val2, val3, val4);

}
