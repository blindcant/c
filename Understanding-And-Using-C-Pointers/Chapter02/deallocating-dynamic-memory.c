#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int* iPtr;

	puts("# Memory Deallocation");
	iPtr = malloc(sizeof(int));

	*iPtr = 2;
	printf("The value of iPtr is %d\n", *iPtr);
	printf("The address of iPtr is %p\n",(void*)  &iPtr);
	printf("The address iPtr points to is %p\n", (void*) iPtr);

	puts("## Using free");
	puts("free(iPtr);");
	/*
	 * Return the memory back to the heap.
	 * Do not use after free, this is an invalid call.
	 */ 
	free(iPtr);

	/*
	 * If we don't update the pointer value, it becomes a dangling pointer. It points to garbage
	 * Good practice is to allocate to NULL after free.
	 */
	iPtr = NULL;

	return EXIT_SUCCESS;
}
