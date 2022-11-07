#include <stdio.h>

int main()
{
    int state = 0;
    int i = 0;
    char input[30] = {"abbb"};   //ERROR
    // char input = {"aabb"};
    switch (state)
    {
        case 0:
            if(input[i]=='a') { state = 1; i++; }
            else if(input[i]=='b') { state = 0; i++;}
            else {state = -1; i++;}
            break;

        case 1:
            if(input[i]=='a') { state = 1; i++; }
            else if(input[i]=='b') { state = 0; i++;}
            else {state = -1; i++;}
            break;
        
        default:
            break;
    }
    // while(1){

    // }
    return 0;
}
    // value = atof(str); //str to float
    // value = atoi(str); //str to integer