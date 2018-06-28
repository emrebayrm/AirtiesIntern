#include <stdio.h>
#include <stdlib.h>

int shuffle(int a[3]){
	int t ;
	for (int i = 0; i < 3; ++i)
	{
		a[i] = 0;
	}
	t = rand() % 3;
	a[t] = 1;
	
	return t;
}

int main(int argc, char const *argv[])
{
	srand(time(NULL));
	int totalWin = 0;
	int count = 10000;
	int arr[3];
	int ch;
	int deg;
	int truee;
	for (int i = 0; i < count; ++i)
	{
		ch = rand() % 3;
		truee = shuffle(arr);
		deg = rand() % 2;

		if (truee == ch && deg == 0) 
		{
			printf("congrat \n");
			totalWin++;
		}
		else if(truee != ch && deg == 1)
		{
			printf("congrat \n");
			totalWin++;
		}
		printf("araba %d secilen %d  degistir di mi ? %c  olasılık %lf\n",truee,ch, deg ?'e':'h' , (1.0 * totalWin )/ i );
		
	}



	return 0;
}