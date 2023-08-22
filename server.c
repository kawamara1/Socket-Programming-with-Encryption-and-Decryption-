// Header files for socket programming
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define PORT 8000

#define MESS_LEN 500


char *decrypt(char *cipher_text, int key){
    // Decrypt the text using key and return the plain text
    char * temp= (char*) malloc(500);

    for(int i=0;cipher_text[i] != '\0';i++){
        temp[i] = cipher_text[i];
        //checking if characters are uppercase
        if(isupper(temp[i])){
                temp[i] = (temp[i] - 'A' - key + 26) % 26 + 'A';
        }
        //checking if characters are lowercase
        if (islower(temp[i])) {
                temp[i] = (temp[i] - 'a' - key + 26) % 26 + 'a';
        }
            //checking if character is a number
        if(isdigit(temp[i])){
                temp[i] = (temp[i] - '0' - key + 10) % 10 + '0';
        }
    }
    return temp;
}

int toInt(char *key){
    // Convert the key of character array to int and return it
    int int_key;
    int_key = atoi(key);
    return int_key;
}


char *split(char *text){
    // 1. Split the key and encrypted text
    // 2. Convert the key to int
        char * key = (char*) malloc(2);
        int count = 0;
        for (; *text!=':'; text++){
                *key = *text;
                key++;
                count++;
        }
        key-= count;
        int keyNum = toInt(key);
        text++;
        count = 0;
        char * eText = (char*) malloc(strlen(text)* sizeof(char));
        for (; *text!='\0'; text++){
                *eText = *text;
                eText++;
                count++;
        }
        eText-=count;
    // 3. Call the decrypt function with encrypted text and key as an argument
       char * decrypted = decrypt(eText, keyNum);
    // 4. Return the plain text
       return decrypted;
}


int main(void)
{
    // 1. Create Socket
     int socket_desc = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Initialize the struct variables
   
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind
   
    bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Create client_addr struct and get the length of struct client_addr
   
    struct sockaddr_in client_addr;
    int client_struct_length = sizeof(client_addr);

    // 5. Wait for the Client to send the message and receive it using recfrom() method, and return
// type is int (Length of the message)

    char * message = (char*) malloc(500);
   
 //int n = recvfrom(socket_desc, encrypted_text, sizeof(encrypted_text), 0, (struct sockaddr*)

    int n = recvfrom(socket_desc, message, MESS_LEN, 0, (struct sockaddr*) &client_addr, &client_struct_length);

    // 6. Call the split function and inside a split function, call the decrypt function to get 
//plain text

    message = split(message);


    // 7. Print the plain text
    printf("Data Received: %s\n", message);
}
