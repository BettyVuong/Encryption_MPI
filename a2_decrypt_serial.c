#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

int isValidInDict(char * filename, char * inputwrd);
char * getCrypt(char * cryptPath);
void decryptWord(char * cryptwrd, char * dictPath);
char * uniqueLetters(char * cryptwrd);
void permutate(char * tempwrd, char * cryptwrd, char * dictPath, int left, int right, int lenLetters, int rank);
char * decrypt(char * str,char * encrypt_dictionary);
double calcTime(struct timeval start);

int flag = 0;

int main(int argc, char *argv[]){
    //Start timer
    struct timeval	start;
    gettimeofday(&start, NULL);
    //getting path for files via CLI
    char* cryptPath = argv[1];
    char* dictPath = argv[2];
    
    //getting word to decrypt
    char * inputwrd = getCrypt(cryptPath);
    if (inputwrd == NULL){
        return 0;
    }

    //decrypting the word
    decryptWord(inputwrd, dictPath);
    if (flag == 0){
        printf("No decryption found for this dictionary.\n");
    }
    free(inputwrd); //leak patch test
    //end timer
    double elapsed = calcTime(start);

    //print to txt file
    printf("%.4f,", elapsed);

}

char * getCrypt(char * cryptPath){
    FILE *fptr = fopen(cryptPath,"r");
    if(fptr==NULL){
        printf("Cannot open encrypted file\n");
        return NULL;
    }
     //get file size
    fseek(fptr, 0, SEEK_END);
    long fSize = ftell(fptr);
    rewind(fptr);

    char * cryptwrd = malloc(sizeof(char)*(fSize + 1));

    size_t bytesRead = fread(cryptwrd, 1, fSize, fptr);
    if (bytesRead != fSize) {
        printf("Error reading encrypted file\n");
        fclose(fptr);
        free(cryptwrd);
        return NULL;
    }
    cryptwrd[bytesRead] = '\0'; // Null-terminate the string 
    cryptwrd[strcspn(cryptwrd, "\r\n")] = 0;

    fclose(fptr);
    return cryptwrd;
}

int isValidInDict(char * filename, char * inputwrd){
    FILE *fptr = fopen(filename,"r");
    if(fptr==NULL){
        printf("Cannot open dictionary file\n");
        return -1;
    }

    //loading file to string
    char copy[strlen(inputwrd)+1];
    strcpy(copy, inputwrd);
    char *token = strtok(copy, " ");

    int count = 0;
    int realWords = 0;
    // int index = 0;
    //printf("Token:%s\n",token);
    while(token!=NULL){
        int found = 0;
        rewind(fptr);
        count++;

        char dictionary[255];

        while(fgets(dictionary, sizeof(dictionary), fptr)){
            //remove newline characters
            dictionary[strcspn(dictionary, "\r\n")] = '\0';
            //compare dict string to input
                if(strcasecmp(dictionary, token) == 0){
                    found = 1;
                    realWords++;
                    break;
                } 
            }
            if(!found){
                fclose(fptr); //leak patch test
                return 0;
            }
            token = strtok(NULL, " ");
    }
    
    fclose(fptr);
    //word is found in dictionary
    return 1;
}

char * uniqueLetters(char * cryptwrd){
    int len = strlen(cryptwrd);

    //lowercase the encrypted word
    for(int i = 0; i < len; i++){
        cryptwrd[i] = tolower(cryptwrd[i]);
    }

    //find unique characters
    char * letters = malloc(len+1);
    if (letters == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    letters[0] = '\0'; //initialize empty string
    for (int i = 0; i < len; i++) {
        char letter = cryptwrd[i];
        if(letter == ' '){
            continue; //skip spaces
        }
        if(strchr(letters, letter) == NULL){
            strncat(letters, &letter, 1); //add letter to unique letters
        }
    }
    letters[strlen(letters)] = '\0'; //null terminate string
    return letters;
}

void decryptWord(char * cryptwrd, char * dictPath){
    // get unique characters from the encrypted word
    char * letters = uniqueLetters(cryptwrd);

    //separate cases by each letter from the unique letters to bruteforce the decryption
    int lenLetters = strlen(letters);
    //loop through each letter and change case
    for(int i = 0; i < lenLetters; i++){
        //copy the original encrypted word
        char * tempwrd = malloc(lenLetters + 1);
        strcpy(tempwrd, letters);
        //shuffle to the next unique letter
        char temp = tempwrd[0];
        tempwrd[0] = tempwrd[i];
        tempwrd[i] = temp;
        //permutations for the starting unique letter
        permutate(tempwrd, cryptwrd, dictPath, 1, lenLetters-1, lenLetters, i);
        free(tempwrd);

    }
    
    free(letters);

}

void permutate(char * tempwrd, char * cryptwrd, char * dictPath, int left, int right, int lenLetters, int rank){

    //base case
    if (left == right){
        //after generating a permutation, check for valid word in dictionary since it's a possible key
        char * inputwrd = decrypt(cryptwrd, tempwrd);
        //decryption testing
        if (inputwrd == NULL){
            return;
        }

        //opening dictionary and checking for valid words
        int validWrd = isValidInDict(dictPath, inputwrd);
        if(validWrd == 1){
            fprintf(stderr,"The decrypted word is: %s\n", inputwrd);
            flag = 1;

        }
        free(inputwrd); //LEAK PATCH TEST
        return;

    } else{ //swapping characters
        for (int i = left; i <= right; i++){
            //swap
            char temp = tempwrd[left];
            tempwrd[left] = tempwrd[i];
            tempwrd[i] = temp;
            //call permutate recursively
            permutate(tempwrd, cryptwrd, dictPath, left + 1, right, lenLetters,i);
            //backtrack (swap back)
            temp = tempwrd[left];
            tempwrd[left] = tempwrd[i];
            tempwrd[i] = temp;
        }

    }
}

//Takes a string and decrypts it based on the provided encrypt dictionary
char * decrypt(char * str,char * encrypt_dictionary){
    int len = strlen(str);
   
    //lowercase the encrypted word
    for(int i = 0; i < len; i++){
        str[i] = tolower(str[i]);
    }

    //find unique characters
    char * input_dictionary = malloc(len+1);
    input_dictionary[0] = '\0'; //initialize empty string
    for (int i = 0; i < len; i++) {
        char c = str[i];
        if(strchr(input_dictionary, c) == NULL && !isspace(c)){ {
            strncat(input_dictionary, &c, 1); //add letter to unique letters
        }
    }
    }
    //Null terminate str
    input_dictionary[strlen(input_dictionary)] = '\0';

    //Temp str to not mess up the origianl
    char * tempstr = malloc(sizeof(char)*strlen(str)+1);
    tempstr[len] = '\0';
    strcpy(tempstr,str);

    //iterate through the string, and when a letter in the string matches one at index j in the input dictionary
    //switch the character in the string for the character at index j in the encrypt dictionary 
    //then break since this new letter may find other matches
    for(int i = 0;i<len;i++){
        for(int j = 0;j<strlen(input_dictionary);j++){
            if(tempstr[i]==input_dictionary[j]){
                tempstr[i]=encrypt_dictionary[j];
                break;
            }
        }
    }
    free(input_dictionary);
    return tempstr;
} 
//timing function from class example
double calcTime(struct timeval start){
    
    long long		startusec, endusec;
    struct timeval	end;
    
    gettimeofday(&end, NULL);
    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec = end.tv_sec * 1000000 + end.tv_usec;
    return (double)(endusec - startusec) / 1000000.0;
}