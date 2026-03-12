#include <stdio.h>
#include <KV.h>

int main() {
	kv_t *table = kv_init(3);
	printf("%p\n", table);

	printf("capacity = %ld\n", table->capacity);
}
