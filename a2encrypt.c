//Cis3090 A1
//Student Numbers: 1271673 & 1278122
//Names: Betty Vuong & Justin Ivanovski

#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <semaphore.h>
#include <ctype.h>

char * input_dictionary;
char * encrypt_dictionary;

//Could also just not pass input_dictionary in and leave it global not sure 
int input_check(char char_to_insert);

int insert_to_dictionary(char char_to_insert);

int randomize_dictionary();

int encrypt_string(char * str);

int main(int argc, char *argv[]){

//gets the string to encrypt from command line
    if(argv==NULL){
        printf("Command line argument issue");
        return 1;
    }

    

    //checks if it can be encrypted, can not encrypt a single character
    char * str = argv[1];

    if(strlen(str)<=1){
        return 1;
    }

    //makes the string lowercase and populates input dictionary string
    for(int i = 0;i<strlen(str);i++){
        str[i] = tolower(str[i]);
        int check = input_check(str[i]);
        if(check==1){
            insert_to_dictionary(str[i]);
        }
    }

    //shuffles the input dictionary
    randomize_dictionary();

    //encrypts the string based on the encrypt dictionary
    encrypt_string(str);

    
    return 0;
}


//check if the input dictionary exists and iterate through it to make sure a character isn't in it
//before adding new unique character
int input_check(char char_to_insert){
    int charFound = 1;
    if(input_dictionary == NULL){
        return 1;
    }
    
    for(int i = 0;i<strlen(input_dictionary);i++){
        if(char_to_insert==input_dictionary[i]){//if the character is already in the input dict
            charFound = 0;
        }
    }

    

    return charFound;
}

//populates the dictionary
int insert_to_dictionary(char char_to_insert){
    //ensures its a valid character
    if(!isalpha(char_to_insert) && char_to_insert !='\''){
        return -1;
    }else if(isspace(char_to_insert)){
        //Do not insert spaces, but not necessarily a fail
        return 0;
    }
    char_to_insert = tolower(char_to_insert);
    
    //reallocate space for input dictionary to allow for the new character, insert it, then null terminate
    char * temp;
    if(input_dictionary==NULL){//if the dictionary does not exist yet
        input_dictionary = malloc(sizeof(char)*(2));
        // strcat(input_dictionary,char_to_insert);
        input_dictionary[0] = char_to_insert;
        input_dictionary[1] = '\0';
        return 1;
    }else{//otherwise add more sapce
        temp = malloc (sizeof(char)*(strlen(input_dictionary)+2));
    }

    //Make input dictionary into the updated temp dictionary
    int len = strlen(input_dictionary);
    temp = strcpy(temp,input_dictionary);
    temp[len] = char_to_insert;
    temp[len + 1] = '\0';
    

    free(input_dictionary);
    input_dictionary = temp;
    // temp = strcat(temp,char_to_insert);

    
    return 1;
}


//Shuffles the characters in the input dictionary to create the encrypt dictionary
int randomize_dictionary(){
    srand(time(NULL));//to generate random num
    int len = strlen(input_dictionary);
    if(encrypt_dictionary==NULL){//malloc encrypt dictionary if it does not exist
        encrypt_dictionary = malloc(sizeof(char) * (len+1));
    }
    
    //set encrypt dict to input dict
    strcpy(encrypt_dictionary,input_dictionary);



    //iterate through the encrypt dict
    for(int i = 0; i < len;i++){
        int j = rand() % (i + 1);//generate random index

        //swap the current char with the char at the random index
        char temp = encrypt_dictionary[i];
        encrypt_dictionary[i] = encrypt_dictionary[j];
        encrypt_dictionary[j] = temp;
    }

    return 1;

}

//encrypts the string based on the encrypt dictionary
int encrypt_string(char * str){
    //creates temp str to encrypt
    int len = strlen(str);
    char * temp = malloc(sizeof(char) * (len+1));
    temp[len] = '\0';

    do {
        randomize_dictionary();//randomizes the input dictionary to create the encrypt dictionary

        //iterates through the string
        for(int i = 0; i < len; i++){
            if(isspace(str[i])){//leaves spaces where they are
                temp[i] = ' ';
            } else {
                //if its not a space, it attempts to match the current char with one from the input dictionary at index j
                //then swaps the character in the string with the character at j in the encrypt dictionary
                for(int j = 0; j < strlen(input_dictionary); j++){
                    if(str[i] == input_dictionary[j]){
                        temp[i] = encrypt_dictionary[j];
                        break;
                    }
                }
            }
        }
    } while(strcasecmp(temp, str) == 0); //if the encrypt dictionary ends up the same as the input dictionary and creates the same string, repeat encryption process
    

//write the output to the ciphertext file
    FILE *fp = fopen("ciphertext.txt", "w");
    if (fp == NULL) {
        perror("Failed to open file");
        return 1;
    }

    // write to the file
    fprintf(fp, "%s",temp);
   
    // close the file
    fclose(fp);

    printf("%s\n",temp);
    return 1;
}