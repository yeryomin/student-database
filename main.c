#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

struct __attribute__((__packed__)) student {
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

int data_save(const char *file, struct student *data, int count)
{
	int err = 0;

	int fd = open(file, O_CREAT | O_TRUNC | O_RDWR, 000666 );
	if (fd < 0) {
		printf("error opening data file (%s): %i (%s)\n",
				file, errno, strerror(errno));
		err = fd;
		goto exit;
	}

	ssize_t bytes = write(fd, data, sizeof(struct student) * count);
	if (bytes < 0) {
		printf("error writing to file (%s): %i (%s)\n",
				file, errno, strerror(errno));
		err = bytes;
		goto exit;
	}
	printf("%ld bytes written to %s\n", bytes, file);

exit:
	if (fd > 0)
		close(fd);

	return err;
}

int data_read(const char *file, struct student *data)
{
	int err = 0;
	struct student *tmp = NULL;
	unsigned int sbytes = sizeof(struct student);
	int read_count = 0;

	int fd = open(file, O_RDWR, 000666 );
	if (fd < 0) {
		printf("error: failed opening data file (%s): %i (%s)\n",
				file, errno, strerror(errno));
		err = fd;
		goto exit;
	}

	data = (struct student *)malloc(sbytes);
	if (!data) {
		printf("error: failed to allocate %d bytes\n", sbytes);
		err = -1;
		goto exit;
	}

	ssize_t bytes = 1;
	ssize_t total_bytes = 0;

	while (bytes > 0) {
		bytes = read(fd, data, sbytes);
		if (bytes == -1) {
			printf("error: reading %s failed with: %d (%s)\n",
					file, errno, strerror(errno));
			err = bytes;
			goto exit;
		}

		if (bytes == 0)
			break;

		if (bytes < sbytes) {
			printf("error: read %ld out of %d bytes expected\n",
					bytes, sbytes);
			err = bytes;
			goto exit;
		}

		read_count++;
		total_bytes += bytes;

		tmp = (struct student *)realloc(data, sbytes * (read_count + 1));
		if (!tmp) {
			printf("error: failed to allocate %d bytes\n",
					sbytes * (read_count + 1));
			err = -1;
			goto exit;
		}
		data = tmp;
	}

	printf("read %ld bytes from %s\n", total_bytes, file);
	close(fd);
	return read_count;

exit:
	if (fd > 0)
		close(fd);
	return err;
}

int main()
{
	int err = 0;
	int count = 0;
	char *file = "data.bin";
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

	err = data_save(file, s, count);

	struct student *data = NULL;
	int read_count = 0;
	read_count = data_read(file, data);
	printf("read %d student records from %s\n", read_count, file);

exit:
	free(s);
	if (data)
		free(data);

	return err;
}
