#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#define ArrayCount(array) (sizeof(array) / sizeof((array)[0]))

#define ArrayForeach(val, array) \
for (int i = 0; i < ArrayCount(array); ++i) \
for (bool b = true; b; b = false) \
for (val = (array)[i]; b; b = false)

#define ArrayForeachRef(val, array) \
for (int i = 0; i < ArrayCount(array); ++i) \
	for (bool b = true; b; b = false) \
		for (val = &(array)[i]; b; b = false)

enum ItemType
{
	Item_Int = 0,
	Item_Int64,
	Item_Char,
	Item_String,
};

struct Person
{
	const char* name;
	int age;
};

struct PrintItem
{
	ItemType type;
	uint64_t value;
};

void Print(int numItems, ...)
{
	va_list argList;
	va_start(argList, numItems);

	while (numItems--)
	{
		PrintItem item = va_arg(argList, PrintItem);
		
		switch (item.type)
		{
			case Item_Int:
			{
				printf("%d\n", (int)item.value);
			} break;

			case Item_Int64:
			{
				printf("%#llx\n", item.value);
			}

			case Item_Char:
			{
				printf("%c\n", (char)item.value);
			} break;

			case Item_String:
			{
				printf("%s\n", (const char*)item.value);
			} break;
		}
	}

	va_end(argList);
}

int main(void) {
	int arr[] = { 1, 3, 2, 4, 3 };

	//Print();
	Person p = { "Yuval", 17 };

	Print(4, PrintItem{ Item_Int, 5 }, PrintItem{ Item_Char, 'a' }, PrintItem{ Item_String, (uint64_t)"Hello World" },
		PrintItem{ Item_Int64, 0xFFFFFFFFFFFFFFFF });

	return 0;
}