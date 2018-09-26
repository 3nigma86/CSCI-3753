#include <stdio.h>		
#include <stdlib.h>		
#include <fcntl.h>		

#define BUFFER_SIZE 1024

int main() {
	char option, buffer[BUFFER_SIZE];
	int offset, whence;
	int file = open("/dev/simple_character_device", O_RDWR);

	printf("\n\nTime to test the device driver!!!\n");

	while(1) {
		printf("\n\nOptions:\nPress r to read from the device\nPress w to write to the device\nPress s to seek through device\nPress e to exit from the device\nPress anything else to keep reading or writing from the device\n\nEnter option: ");
		scanf("%c", &option);
		switch (option) {
			case 'r':
			case 'R':				
				read(file, buffer, BUFFER_SIZE);
				printf("Device output: %s\n", buffer);
				while (getchar() != '\n');
				break;
			case 'w':
			case 'W':
				printf("Enter data to write to the device: ");
				scanf("%s", buffer);
				write(file, buffer, BUFFER_SIZE);
				while (getchar() != '\n');
				break;
			case 's':
			case 'S':
				printf("\nEnter the location to go to using offset and whence. For the whence, \nEnter 0 to offset from the start of file. \n Enter 1 to offset from the curent position in the file \n Enter 2 to offset from the end of file.\n");
				printf("\nEnter offset: ");				
				scanf("%d", &offset);
				printf("\nEnter Whence (0, 1, or 2): ");
				int  correct = 0;
				while (!correct){
					scanf("%d", &whence);
					if (whence == 0 || whence == 1 || whence == 2)
						correct = 1;
				}
				int pos = llseek(file, offset, whence);	
				printf("You are at position %d\n", pos);
				break;
				
			case 'e':
			case 'E':
				printf("Exiting test_app\n\nGoodbye\n");
				return 0;
			default: 
				while (getchar() != '\n');
		}
	}
	close(file);
	return 0;
}