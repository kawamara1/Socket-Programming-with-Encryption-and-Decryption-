
// Header files for socket programming
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PORT 8000

// Max Length of the plain text
#define MESS_LEN 500

int get_len_key(int key){
    // Return the number of digits of the key
    int count = 0;
    while (key != 0){
          count++;
          key /= 10;
    }
    return  count;
}

char *toString(int key)
{
    // 1. Get the number of digits of the key
    int digits = get_len_key(key);
    // 2. Convert into a character pointer and return it.
    char * str = (char*) malloc(digits);
    sprintf(str, "%d", key);
    return str;
}

char *encrypt(char *plain_text, int key)
{
    // 1. Convert the plain_text to encrypt_text using key
    char temp,encrypted_text[MESS_LEN];
    for (int i = 0; plain_text[i] != '\0'; ++i) {

    temp = plain_text[i];

      // checking if text contains lower case characters
    if (islower(temp)) {
        temp = (temp - 'a' + key) % 26 + 'a';
    }
      // checking if text contains uppercase characters
    if (isupper(temp)) {
        temp = (temp - 'A' + key) % 26 + 'A';
    }

      // checking numbers
    if (isdigit(temp)) {
        temp = (temp - '0' + key) % 10 + '0';
    }

    // adding encoded answer
    encrypted_text[i] = temp;
    }

    // 2. Convert the key (int) to a Character pointer using the toString() method.
    char *key_ptr = toString(key);

    // 3. Concat key:encrypt_text
    char * finalMessage = (char*) malloc(3+(strlen(encrypted_text) * sizeof(char)));
    sprintf(finalMessage,"%s:%s", key_ptr, encrypted_text);
    // 4. Return the key:encrypt_text to main() method
    return finalMessage;
}

int main(void){

    // 1. Create socket
   
   int socket_desc = socket(PF_INET, SOCK_DGRAM, 0);  

    // 2. Initialize the struct variables
   
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(PORT);  
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Take a key and plain plain_text as an input (Key: Int, Plain plain_text: Pointer Array of 
//character)
   
    int key = 0;
    char *plain_text = (char*)malloc(MESS_LEN * sizeof(int));;

    printf("Plain text: ");
    fgets(plain_text, MESS_LEN, stdin);
    //scanf("%s", plain_text);

    printf("\nKey: ");
    scanf("%d", &key);

    // 4. Encrypt the plain text using key
  
     char * encrypted_text = encrypt(plain_text,key);

    // 5. Send the encrypt_text to Server using the sendto() method.
     
     sendto(socket_desc, encrypted_text, strlen(encrypted_text), 0, (struct sockaddr*) &server_addr, sizeof(server_addr));

    // 6. Print the encrypt_text
    
    printf("Data Sent: %s\n", encrypted_text);
}
