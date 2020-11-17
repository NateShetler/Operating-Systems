// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:_____________________________________ Date:_____________

//filesys.c
//Based on a program by Michael Black, 2007
//Revised 11.3.2020 O'Neil

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int i, j, size, noSecs, startPos;

	// This block will catch any errors with input arguments

	if (argc == 2) // If there is only 1 extra argument (should be L)
	{
		if (*argv[1] == 'P' || *argv[1] == 'D' || *argv[1] == 'M') // If user enters one of these commands without a filename after it
		{
			fprintf(stderr, "Please include a filename. Program quitting...\n");
			return 0;
		}
		else if(*argv[1] != 'L') // If the input is not L, it is not valid
		{
			fprintf(stderr, "Incorrect input. Program quitting...\n");
			return 0;
		}
	}
	else if (argc == 3) // Make sure enters valid command
	{
		if (*argv[1] != 'P' && *argv[1] != 'D' && *argv[1] != 'M')
		{
			fprintf(stderr, "Please enter a valid command.Program quitting...\n");
			return 0;
		}
	}
	else if (argc > 3) // If the user enters too many arguments
	{
		fprintf(stderr, "Please do not include extra arguments. Correct input includes the following:\n");
		fprintf(stderr, "./filesys D filename\n");
		fprintf(stderr, "./filesys L\n");
		fprintf(stderr, "./filesys M filename\n");
		fprintf(stderr, "./filesys P filename\n\n");
		fprintf(stderr, "Program quitting...\n");
		return 0;

	} 

	//open the floppy image
	FILE* floppy;
	floppy=fopen("floppya.img","r+");
	if (floppy==0)
	{
		printf("floppya.img not found\n");
		return 0;
	}

	//load the disk map from sector 256
	char map[512];
	fseek(floppy,512*256,SEEK_SET);
	for(i=0; i<512; i++)
		map[i]=fgetc(floppy);

	//load the directory from sector 257
	char dir[512];
	fseek(floppy,512*257,SEEK_SET);
	for (i=0; i<512; i++)
		dir[i]=fgetc(floppy);


	// Print the disk map unless the command is the 'L' command
	if ((argc == 1) || (argc >= 2 && *argv[1] != 'L'))
	{
		// Char array for filename
		char filename[strlen(argv[2])];

		// To keep track if name is found on the disk
		bool fileFound = false;

		// Will tell if the file is a text file or not
		bool isTextFile = false;

		// To store sector number
		int sectorNumber = 0;

		// to store length
		int fileLength = 0;

    	//print disk map
		printf("Disk usage map:\n");
		printf("      0 1 2 3 4 5 6 7 8 9 A B C D E F\n");
		printf("     --------------------------------\n");
		for (i=0; i<16; i++) {
			switch(i) {
				case 15: printf("0xF_ "); break;
				case 14: printf("0xE_ "); break;
				case 13: printf("0xD_ "); break;
				case 12: printf("0xC_ "); break;
				case 11: printf("0xB_ "); break;
				case 10: printf("0xA_ "); break;
				default: printf("0x%d_ ", i); break;
			}
			for (j=0; j<16; j++) {
				if (map[16*i+j]==-1) printf(" X"); else printf(" .");
			}
			printf("\n");
		}

		// To indicate that the file being searched for has already been found
		// (To prevent sectorNumber from being set to the wrong value)
		bool alreadyFound = false;

    	// print directory
		printf("\nDisk directory:\n");
		printf("Name    Type Start Length\n");
    	for (i=0; i<512; i=i+16) {

			if (dir[i]==0) break;

			for (j=0; j<8; j++) {
				if (dir[i+j]==0)
				{
					printf(" "); 
				} 
				else
				{
					printf("%c",dir[i+j]);

					// Only write characters to filename variable if it is less than
					// or equal to the length of the filename entered on the command line
					if (j < strlen(argv[2])) 
					{
						filename[j] = dir[i+j];
					}
				} 
			}

			// If a filename was passed
			if (argc == 3)
			{
				// Check to see if the filename passed exists on the disk
				if (strcmp(filename, argv[2]) == 0)
				{
					// The file was found, set boolean to true
					fileFound = true;

					// Check to see if the file is a text file, if so, set boolean to true
					if ((dir[i+8]=='t') || (dir[i+8]=='T'))
					{
						isTextFile = true;
					}
				}
			}

			if ((dir[i+8]=='t') || (dir[i+8]=='T')) printf("text"); else printf("exec");
			printf(" %5d %6d bytes\n", dir[i+9], 512*dir[i+10]);

			// Store the sectorNumber and length for use with P
			if (fileFound == true && isTextFile == true && alreadyFound == false)
			{
				sectorNumber = dir[i+9];
				fileLength = 512*dir[i+10];
				alreadyFound = true;
			}
			
		}

		// If the command is M, P, D
		if (argc == 3)
		{	
			if (*argv[1] == 'P') // If the command is 'P'
			{	
				// If the file was not found, inform the user
				if (fileFound == false)
				{
					fprintf(stderr, "Error, file not found. Program quitting...\n");
				}
				else
				{
				
					if (isTextFile == false) // If the found is not a text file, inform user
					{
						fprintf(stderr, "The file was not a text file. Program quitting...\n");
						return 0;
					}
					else
					{
						printf("The file was found on the disk.\n");

						//load the file from the correct sector
						char fileBuffer[fileLength];
						fseek(floppy,512*sectorNumber,SEEK_SET);
						for(i=0; i<512; i++)
							fileBuffer[i]=fgetc(floppy);

						// For formatting purposes
						printf("\n");

						// Print file one character at a time
						for (i=0; i<fileLength; i=i+1) 
						{
							if (fileBuffer[i]==0) break;

							// Print character
							printf("%c",fileBuffer[i]);
						}
						
					}
				}
			}
			else if (*argv[1] == 'M')
			{
				if (fileFound == true) // If the filename already exists, quit the program
				{
					fprintf(stderr, "Duplicate or invalid name. Program quitting...\n");
				}
				else
				{

					printf("Creating new file with filename: ");
					printf("%s", argv[2]);
					printf("\n");
				}
				
			}
			
		}

	}
	else // For 'L' command
	{
		// This keeps track of the lenght of the filename
		int counter;

		// Keeps track of the total amount of bytes taken up
		int totalTakenUp = 0;

		// print directory
		printf("\nDisk directory:\n");
		printf("File     Length\n");
    	for (i=0; i<512; i=i+16) {

			// Reset the counter
			counter = 0;

			// Print the filename
			if (dir[i]==0) break;
			for (j=0; j<8; j++) {
				if (dir[i+j]!=0)
				{
					printf("%c",dir[i+j]);
					++counter;
				} 	
			}

			// Print the file extension
			if ((dir[i+8]=='t') || (dir[i+8]=='T')) printf(".t"); else printf(".x");

			// Fix spacing using the counter
			for (int i = 0; i < (8 - (counter +  4)); ++i)
			{
				printf(" ");
			}

			//printf("%d", counter);

			// Print the size
			printf(" %6d bytes\n", 512*dir[i+10]);

			totalTakenUp += 512*dir[i+10];
		}

		printf("\nTotal amount of space taken up by files (in bytes): ");
		printf("%d", totalTakenUp);
		printf("\n");

		printf("Free space remaining (in bytes): ");
		printf("%d", ((512*511) - totalTakenUp));
		printf("\n");
	}


/*
	//write the map and directory back to the floppy image
    fseek(floppy,512*256,SEEK_SET);
    for (i=0; i<512; i++) fputc(map[i],floppy);

    fseek(floppy,512*257,SEEK_SET);
    for (i=0; i<512; i++) fputc(dir[i],floppy);
*/
	fclose(floppy);
}
