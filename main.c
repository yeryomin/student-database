#include <stdio.h>
#include <stdlib.h>

struct student {
	char name[20];
	char last_name[20];
	unsigned int course;
	char group[10];
};

int main()
{
	struct student s = {
		.name = "John",
		.last_name = "Doe",
		.course = 1,
		.group = "XYZ132"
	};

	printf( "Name                |"
		"Last name           |"
		"Course  |"
		"Group\n");
	printf("%-20s|%-20s|%-8d|%-10s\n",
		s.name, s.last_name, s.course, s.group);

	return 0;
}
