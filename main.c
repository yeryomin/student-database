#include <stdio.h>
#include <stdlib.h>

struct student {
	char name[20];
	char last_name[20];
	unsigned int course;
	char group[10];
};

void data_input(struct student *data, int count)
{
	int idx = 0;

	while(count-- > 0) {
		printf("Enter student data (%d) > ", idx);
		scanf("%20s %20s %d %10s",
			(data + idx)->name,
			(data + idx)->last_name,
			&(data + idx)->course,
			(data + idx)->group
		);
		idx++;
	}
}

void data_show(struct student *data, int count)
{
	int idx = 0;

	printf( "Name                |"
		"Last name           |"
		"Course  |"
		"Group\n");

	while(count-- > 0) {
		printf("%-20s|%-20s|%-8d|%-10s\n",
			(data + idx)->name,
			(data + idx)->last_name,
			(data + idx)->course,
			(data + idx)->group
		);
		idx++;
	}
}

int main()
{
	int count = 0;
	struct student *s = NULL;

	printf("Please give a number of students you want to enter > ");
	scanf("%d", &count);

	s = (struct student *)malloc(sizeof(struct student) * count);
	if (!s) {
		printf("memory allocation failed!\n");
		return -1;
	}

	data_input(s, count);
	data_show(s, count);

	free(s);

	return 0;
}
