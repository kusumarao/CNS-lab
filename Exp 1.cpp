#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define MAX_LENGTH 1000
void caesar_cipher_encrypt(char *message, int key)
{
	int i;
	for(i = 0; message[i] != '\0'; i++)
	{
		if(isalpha(message[i]))
		{
			char base = islower(message[i]) ? 'a' : 'A';
			message[i] = (message[i] - base + key) % 26 + base;
		}
	}
}
void caesar_cipher_decrypt(char *encrypted_message, int key)
{
	caesar_cipher_encrypt(encrypted_message, -key);
}
int main()
{
	char message[MAX_LENGTH];
	int key;
	printf("Enter the message to encrypt: ");
	fgets(message, MAX_LENGTH, stdin);
	message[strcspn(message, "\n")] = '\0';
	printf("Enter the key (an integer): ");
	scanf("%d", &key);
	getchar();
	caesar_cipher_encrypt(message, key);
	printf("\nEncrypted message: %s\n", message);
	caesar_cipher_decrypt(message, key);
	printf("Decrypted message: %s\n", message);
	return 0;
}