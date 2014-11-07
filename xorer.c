#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

void printUsage();
void createInputList();
void createOutput();
int getNumber(char *inputNumberInString);

int doConversion();

char *input, *output;
char **inputList;
int xorNumber = 0x19;
int startPoint = 0, endPoint = 0;
int totalFiles = 0;

int main(int argc, char *argv[]) {
	int i;
	
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-i") == 0) {
			if (i+1 < argc) {
				if (input == NULL) {
					int length = strlen(argv[i+1] + 1);
					input = (char *)malloc(sizeof(char) * length);
					strcpy(input, argv[i+1]);
					i++;
				} else {
					printf("multiple -i specified. Only one file can be processed each time.\n");
					return -1;
				}
			} else {
				printf("-i used but no parameter specified.\n");
				printUsage();
				return -1;
			}
			
		} else if (strcmp(argv[i], "-o") == 0) {
			if (i+1 < argc) {
				if (output == NULL) {
					int length = strlen(argv[i+1] + 1);
					output = (char *)malloc(sizeof(char) * length);
					strcpy(output, argv[i+1]);
					i++;
				} else {
					printf("multiple -o specified. Only one file can be processed each time.\n");
					return -1;
				}
			} else {
				printf("-o used but no parameter specified.\n");
				printUsage();
				return -1;
			}
			
		} else if (strcmp(argv[i], "-x") == 0) {
			if (i+1 < argc) {
				xorNumber = getNumber(argv[i+1]);
				i++;
			} else {
				printf("-x used but no parameter specified.\n");
				printUsage();
				return -1;
			}
		} else if (strcmp(argv[i], "-s") == 0) {
			if (i+1 < argc) {
				startPoint = getNumber(argv[i+1]);
				i++;
			} else {
				printf("-s used but no parameter specified.\n");
				printUsage();
				return -1;
			}
		} else if (strcmp(argv[i], "-e") == 0) {
			if (i+1 < argc) {
				endPoint = getNumber(argv[i+1]);
				i++;
			} else {
				printf("-e used but no parameter specified.\n");
				printUsage();
				return -1;
			}
		}
	}
	
	if (input == NULL) {
		printf("No input file specified, program will convert all files in the directory (exclude subdirectories) and ignore output setting.\n");
		createInputList();
	} else {
		
		if (output == NULL) {
			printf("No output filename specified, output file will be place under \"xorer\" folder.\n");
			createOutput();
		}
	}
	
	if (xorNumber == 0x00) {
		printf("XORNumber is 0, which means there's no conversion between old and new files.\n");
		printUsage();
		
		return 0;
	}
	
	if (input == NULL) {
		if (output != NULL) {
			free(output);
		}
		input = (char *)malloc(sizeof(char) * 256);
		output = (char *)malloc(sizeof(char) * 266);
		for (i = 0; i < totalFiles; i++) {
			strcpy(input, inputList[i]);
			createOutput();
			if (doConversion() != 1) {
				return -1;
			}
		}
	} else {
		if (doConversion() != 1) {
			return -1;
		}
	}
	
	if (inputList != NULL) {
		for (i = 0; i < totalFiles; i++) {
			free(inputList[i]);
		}
	}
	free(inputList);
	free(input);
	free(output);
	
	return 0;
}

void printUsage() {
	printf(
		   "============================================================\n"
		   "* This is a program to encrypt / decrypt file.\n"
		   "* \n"
		   "* Usage:\n"
		   "* -i: Input file name. If you leave this blank, program will convert all files in the directory (exclude subdirectories) and ignore output setting.\n"
		   "* -o: Output file name. If you leave this blank, it will generate the output file in \"xorered\" sub directory.\n"
		   "* -x: XOR parameter. Binary will be XORed by this number. Default is 0x19 (25). You can use 0xXX as a hexadecimal number, 0bXX as a binary number, or just XX as a decimal number.\n"
		   "* -s: Start point parameter (Byte). Default is 0. Program will apply conversion right from this point.\n"
		   "* -e: End point parameter (Byte). Default is 0. While it's 0 it'll be fixed to file size, and program will stop applying encryption before this point.\n"
		   "============================================================\n\n"
		);
}

void createInputList() {
	DIR *dp;
	struct dirent *dir;
	dp = opendir("./");
	while ((dir = readdir(dp)) != NULL) {
		if (dir->d_type != DT_DIR && dir->d_name[0] != '.') {
			totalFiles++;
		}
	}
		
	int i;
	inputList = (char **)malloc(sizeof(char) * 256 * totalFiles);
	for (i = 0; i < totalFiles; i++) {
		inputList[i] = (char *)malloc(sizeof(char) * 256);
	}
	rewinddir(dp);
	i = 0;
	
	while ((dir = readdir(dp)) != NULL) {
		if (dir->d_type != DT_DIR && dir->d_name[0] != '.') {
			strcpy(inputList[i], dir->d_name);
			i++;
		}
	}
	
	closedir(dp);
}

void createOutput() {
	struct stat st = {0};
	if (stat("./xorered", &st) == -1) {
		mkdir("./xorered", 0777);
	}
	
	int length = strlen(input);
	if (output == NULL) {
		output = (char *)malloc(sizeof(char) * (length+10));
	}
	char *buff = "xorered/";
	strcpy(output, buff);
	strcpy(output+8, input);
}

int getNumber(char *inputNumberInString) {
	int ret = 0;
	if (inputNumberInString[0] == '0' && inputNumberInString[1] == 'x') {
		int length = strlen(inputNumberInString);
		char newString[length];
		strcpy(newString, inputNumberInString+2);
		ret = (int)strtol(newString, NULL, 16);
	} else if (inputNumberInString[0] == '0' && inputNumberInString[1] == 'b') {
		int length = strlen(inputNumberInString);
		char newString[length];
		strcpy(newString, inputNumberInString+2);
		ret = (int)strtol(newString, NULL, 2);
	} else {
		ret = (int)atoi(inputNumberInString);
	}
	
	return ret;
}

int doConversion() {
	FILE *fp = fopen(input, "rb");
	if (fp == NULL) {
		printf("Open file failed: %s\n", input);
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	int convStart, convEnd;
	
	if (startPoint > fileSize) {
		convStart = fileSize;
	} else {
		convStart = startPoint;
	}
	if (endPoint <= 0) {
		convEnd = fileSize;
	} else {
		convEnd = endPoint;
	}
		
	int i;
	unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * fileSize);
	for (i = 0; i < fileSize; i++) {
		if (i >= convStart && i < convEnd) {
			data[i] = fgetc(fp) ^ xorNumber;
		} else {
			data[i] = fgetc(fp);
		}
	}
	fclose(fp);
	
	fp = fopen(output, "wb");
	if (fp == NULL) {
		printf("File Open Error: Failed to write file %s\n", output);
		return -1;
	}
	fwrite(data, sizeof(unsigned char), fileSize, fp);
	fclose(fp);
	
	printf("%s file created. XOR value is 0x%x\n", output, xorNumber);
	free(data);
	
	return 1;
}