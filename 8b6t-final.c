#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>


#define VAL_INDEX 6
#define W_INDEX 7
#define TOTAL_COMBINATION 256

//ASSING A NUMBER AS IF IT WAS BASE 3, SO THAT PATTERN COULD BE UNIQUELY IDENTIFIED
int power(int x, int y){
	int i, res = 1;

	for(y; y > 0; y--)
		res *= x;

	return res;
}

//GENERATE 6T CODE
void fnGen6t(int* ia6t){
	int i;
	int sum = 0, weight = 0;

	for(i = 0; i < 6; i++){
		ia6t[i] = rand() % 3 - 1;
		weight = weight + ia6t[i];
		sum = sum + (power(3, 6 - i)) * (ia6t[i] + 1);
	}

	ia6t[VAL_INDEX] = sum;
	ia6t[W_INDEX] = weight;

	if(weight != 0 && weight != 1)
		fnGen6t(ia6t);
}

//GENERTAE THE MAPPING
void fnGen8b6t(int ia8b6tMapping[][8], int iBytes){
	int i, j;
	int ia6t[8];

	for(i = 0; i < iBytes; i++){
		startAgain:
			fnGen6t(ia6t);

		if(i != 0)			
			for(j = 0; j < i; j++)
				if(ia8b6tMapping[j][VAL_INDEX] == ia6t[VAL_INDEX])
					goto startAgain;

		for(j = 0; j < 8; j++)
			ia8b6tMapping[i][j] = ia6t[j];
	}	
}

//DISPLAY THE SINGLE UNIT
void fnDispSignalUnit(int signalUnit){
	if(signalUnit == -1)
		printf("-");
	else if(signalUnit == 0)
		printf("0");
	else	
		printf("+");
}

//CHECK IF THE SIGNAL NEEDS TO BE INVERTED
char* fnCheckInver(int check){
	if(!check)
		return "NO";
	else if(check == 1) 
		return "YES";
	return "NA";
}

//DISPLAY THE SIGNAL MAPPING
void fnShow8b6tMapping(int map[][8], int iTotalBytes, int iInverted){
	int i, j;
	printf("-----------------------------------------------------------------\n");
	printf("|   Signal   |\t Value\t |   Data    |    Weight  |  Inverted   |\n");
	printf("-----------------------------------------------------------------\n");

	for(i = 0; i < iTotalBytes; i++){
		printf("|   ");
		for(j = 0; j < 6; j++)
			fnDispSignalUnit(map[i][j]);
		printf("   |\t  %d\t |    %-4d   |   %4d     |      %s     |", map[i][VAL_INDEX], i, map[i][W_INDEX], fnCheckInver(iInverted));  
		printf("\n");
	}
	printf("-----------------------------------------------------------------\n");
	printf("\n");
}

//DISPLAY THE MAPPING FOR ALL PATTERNS
void fnShowSignalMapping(int iaSignal[][8], int iBytes){

	int i, j;
	int weight = 0;
	int iInversion = 0;
	
	system("clear");

	printf("----------------------------------\n");
	printf("|Signal\t| Data\t|Weight\t|Inverted|\n");
	printf("----------------------------------\n");
	
	for(i = 0; i < iBytes; i++){
		printf("|");
		if(weight == 1 && iaSignal[i][W_INDEX] == 1){
			for(j = 0; j < 6; j++)
				fnDispSignalUnit(iaSignal[i][j] * -1);
			weight = 0;
			iInversion = 1;
		}
		else{
			for(j = 0; j < 6; j++)
				fnDispSignalUnit(iaSignal[i][j]);
			weight += iaSignal[i][W_INDEX];
		}

		printf("\t|  %d\t|%4d\t|%6s\t |", iaSignal[i][VAL_INDEX], iaSignal[i][W_INDEX], fnCheckInver(iInversion));

		iInversion = 0;

		printf("\n");
	}

	printf("----------------------------------\n");
}

//CONVERT DATA TO SIGNAL
void fnDataToSignal(int ia8b61Mapping[][8], int iaSignal[][8], int iBytes){
	int i, j;

	for(i = 0; i < iBytes; i++){
		label3:
		printf("Enter the data byte %d in decimal:\t", i + 1);
		scanf("%d", &iaSignal[i][VAL_INDEX]);

		if(iaSignal[i][VAL_INDEX] < 0 || iaSignal[i][VAL_INDEX] > 255){
			printf("Invalid data. Try again");			
			goto label3;
		}

		for(j = 0; j < 6; j++){
			iaSignal[i][j] = ia8b61Mapping[iaSignal[i][VAL_INDEX]][j];
		}

		iaSignal[i][W_INDEX] = ia8b61Mapping[iaSignal[i][VAL_INDEX]][W_INDEX];
	}
}

//PLOTTING EACH POINT ON THE GRAPH
void fnPlotSignalUnit(int y, int x){
	FILE *fp;
	fp = fopen("8b6tPlot.dat", "a");
	fprintf(fp, "%d\t%d\n", x, y);
	fprintf(fp, "%d\t%d\n", x + 1, y);
	fclose(fp);
}

//PLOT THE SIGNAL ON TGHE GRAPH
void fnPlotSignal(int iaSignal[][8], int iBytes){
	int i, j;
	int weight = 0;
	int iInversion = 0;
	FILE *fp;

	fp = fopen("8b6tPlot.dat", "w");
	fprintf(fp, "%d\t%d\n", 0, 2);
	fprintf(fp, "%d\t%d\n", 0, -2);
	fclose(fp);
	
	for(i = 0; i < iBytes; i++){
		if(weight == 1 && iaSignal[i][W_INDEX] == 1){
			for(j = 0; j < 6; j++)
				fnPlotSignalUnit(iaSignal[i][j] * -1, i * 6 + j);
			weight = 0;
			iInversion = 1;
		}
		else{
			for(j = 0; j < 6; j++)
				fnPlotSignalUnit(iaSignal[i][j], i * 6 + j);
			weight += iaSignal[i][W_INDEX];
		}

		iInversion = 0;
	}
}
		
int main(void){

	int i, j;
	int ia8b6tMapping[TOTAL_COMBINATION][8];
	int maxBytes, iBytes;
	
	fnGen8b6t(ia8b6tMapping, TOTAL_COMBINATION);

	printf("Enter the maximum bytes of data:\t");
	scanf("%d", &maxBytes);
	
	system("clear");

	int iaSignal[maxBytes][8];

	int iChoice;
	int iAssigned = 0;
	
	while(1){
		printf("1. Genrate New 8b6t Mapping\n2. Show 8b6t Mapping\n3. Enter New Input data\n");
		printf("4. Convert Data to Signal\n5. Plot cordinates.\n6. Take input from file.\n");
		printf("7. Exit\n8. Edit code and execute\n");
		printf("Enter your choice:\t");
		scanf("%d", &iChoice);

		system("clear");
		
		switch(iChoice){
			case 1:
				fnGen8b6t(ia8b6tMapping, TOTAL_COMBINATION);

			case 2:
				fnShow8b6tMapping(ia8b6tMapping, TOTAL_COMBINATION, 2);
				break;

			case 3:
				tryAgain:
				iAssigned = 1;
				printf("Enter the amount of data in bytes:\t");
				scanf("%d", &iBytes);
				if(iBytes > maxBytes){
					printf("Maximum data can be %d bytes. PLease try again.\n", maxBytes);
					goto tryAgain;
				}
				fnDataToSignal(ia8b6tMapping, iaSignal, iBytes);

			case 4:
				if(!iAssigned){
					system("clear");
					printf("No previous data found. Enter data first.\n");
					goto tryAgain;				
				}
				fnShowSignalMapping(iaSignal, iBytes);

				break;
			case 5:
				if(!iAssigned){
					system("clear");
					printf("No previous data found. Enter data first.\n");
					goto tryAgain;				
				}
				
				fnPlotSignal(iaSignal, iBytes);
				printf("Coordinates has been plotted in 8b6tPlot.dat file\n");
				system("gnuplot 8b6tPlot.gpl");
				system("eog 8b6tPlot.png");
				break;
			case 6:
				{
					FILE *fp;
					iBytes = 0;
					int val;
					char inFile[30];
					printf("Enter the file name:\t");
					scanf("%s", inFile);

					fp = fopen(inFile, "r");
					if(fp == NULL){
						printf("File %s does not exist.\n", inFile);
						break;
					}
					printf("Data in file is:\n");
					while(fscanf(fp, "%d ", &val) != EOF && iBytes <= maxBytes){
						printf("%d\t", val);
						if(val < 0 || val > 255){			
							continue;
						}

						iaSignal[iBytes][VAL_INDEX] = val;

						for(j = 0; j < 6; j++){
							iaSignal[iBytes][j] = ia8b6tMapping[iaSignal[iBytes][VAL_INDEX]][j];
						}
						iaSignal[iBytes][W_INDEX] = ia8b6tMapping[iaSignal[iBytes][VAL_INDEX]][W_INDEX];
						iBytes++;
					}
					printf("\n");
					iAssigned = 1;
				}
				break;
			case 7:
				exit(0);
			case 8:
				system("gedit 8b6t-final.c");
				system("gcc 8b6t-final.c");
				system("./a.out");
				exit(0);
			default:
				printf("Invalid option. Try again.\n");
		}
	}

	return 0;
}
