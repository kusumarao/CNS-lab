#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#define MAX_LEN 1000
int determinant(int a, int b, int c, int d)
{
	return(a * d - b * c);
	
}
int modulo_inverse(int a, int m)
{
	a = a % m;
	for(int x = 1; x < m; x++)
		if((a * x) % m == 1)
		return x;
		return -1;
	
}
void hill_encrypt(char * message, int key[2][2], int len)
{
	int i, j;
	for(i = 0; i < len; i += 2)
	{
		int char1 = toupper(message[i]) - 'A';
		int char2 = toupper(message[i + 1]) - 'A';
		int enc_char1 = (key[0][0] * char1 + key[0][1] * char2) % 26;
		int enc_char2 = (key[1][0] * char2 + key[1][1] * char2) % 26;
		message[i] = enc_char1 + 'A';
		message[i + 1] = enc_char2 + 'A';
		
	}
}
void hill_decrypt(char * message, int key[2][2], int len)
{
	int det = determinant(key[0][0], key[0][1], key[1][0], key[1][1]);
	int inv_det = modulo_inverse(det, 26);
	int adj_key[2][2];
	adj_key[0][0] = key[1][1];
	adj_key[0][1] = -key[0][1];
	adj_key[1][0] = -key[1][0];
	adj_key[1][1] = key[0][0];
	if(inv_det == -1)
	{
		printf("Modulo inverse does not exist. Cannot decrypt.\n");
		return;
	}
	for(int i = 0; i < 2; i++)
	for(int j = 0; j < 2; j++)
		adj_key[i][j] = (adj_key[i][j] + 26) % 26;
	for(int i = 0; i < len; i += 2)
{
	int char1 = toupper(message[i]) - 'A';
	int char2 = toupper(message[i + 1]) - 'A';
	int dec_char1 = (adj_key[0][0] * char1 + adj_key[0][1] * char2) % 26;
	int dec_char2 = (adj_key[1][0] * char1 + adj_key[1][1] * char2) % 26;
	message[i] = dec_char1 + 'A';
	message[i + 1] = dec_char2 + 'A';
}
}
int main()
{
	char message[MAX_LEN];
	int key[2][2] = {{5, 8}, {17, 3}};
	int len, choice;
	printf("Enter the message to encrypt: ");
	fgets(message, sizeof(message), stdin);
	len = strlen(message);
	if(len % 2 != 0)
	strcat(message, "x");
	len = strlen(message);
	printf("Enter 1 for encryption and 2 for decryption: ");
	scanf("%d", &choice);
	if(choice == 1)
	{
		hill_encrypt(message, key, len);
		printf("\nEncrypted message: %s\n", message);
		
	}
	else if(choice == 2)
	{
		hill_decrypt(message, key, len);
		printf("\nDecrypted message: %s\n", message);
	}
	else
	{
		printf("Invalid choice.\n");
	}
	return 0;
}