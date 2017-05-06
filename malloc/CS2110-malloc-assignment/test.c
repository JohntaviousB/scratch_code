#include <stdlib.h>
#include <time.h>
#include "my_malloc.h"
#include <assert.h>

void check_malloc_3() {
	char* tag = "check_malloc_3 - stress test";
	char* last_data = NULL;
	for (size_t i = 0; i < 1000; i++) {
		size_t size = (i*i) % 1000 + 1;
		char* data = my_malloc(size);
		for (size_t j = 0; j < size; j++)
			data[j] = i*j;
		if (last_data) {
			size_t old_size = ((i - 1)*(i - 1)) % 1000 + 1;
			for (size_t j = 0; j < old_size; j++)
				if (last_data[j] != (char)((i - 1)*j)) {
					return;
				}
			my_free(last_data);
		}
		last_data = data;
	}
	my_free(last_data);
}

int main() {
#if 0
	char * c1 = my_malloc(1);
	char * c2 = my_malloc(1);
	char * c3 = my_malloc(1);
	char * c4 = my_malloc(1);
	assert(c1 && c2 && c3 && c4);
	my_free(c1);
	my_free(c3);
	my_free(c2);
	my_free(c4);
#endif // 0

	check_malloc_3();
	return 0;
}
