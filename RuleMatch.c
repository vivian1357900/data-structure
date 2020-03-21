#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    FILE *input, *output;

    input = fopen("input.txt", "r");   //input file
    output = fopen("output.txt", "a"); //output file

    int number_r, number_c, count;

    //read rule
    fscanf (input, "%d", &number_r);
    char rule[number_r][3];
    for (count = 0; count < number_r; count++)
        fscanf (input, " %c %c %c", &rule[count][0], &rule[count][1], &rule[count][2]);

    //read IP
    fscanf (input, "%d", &number_c);
    char check_IP[number_c][2], check_port[number_c][5];
    for (count = 0; count < number_c; count++)
        fscanf (input, " %c %c", &check_IP[count][0], &check_IP[count][1]);

    //compare IP & rule, then fill in port
    int count_2;
    for (count = 0; count < number_c; count++)
    {
        for (count_2 = 0; count_2 < number_c; count_2++)
        {
            //IP1 & IP2 both same
            if (check_IP[count][0] == rule[count_2][0] && check_IP[count][1] == rule[count_2][1])
            {
                check_port[count][0] = rule[count_2][2];
                check_port[count][1] = '\0';
                break;
            }

            //IP1:same IP2:don¡¦t care
            else if (check_IP[count][0] == rule[count_2][0] && rule[count_2][1] == '*')
            {
                check_port[count][0] = rule[count_2][2];
                check_port[count][1] = '\0';
                break;
            }

            //IP1:don¡¦t care IP2:same
            else if (check_IP[count][1] == rule[count_2][1] && rule[count_2][0] == '*')
            {
                check_port[count][0] = rule[count_2][2];
                check_port[count][1] = '\0';
                break;
            }


        }
        //both different
        if (check_port[count][0] != rule[count_2][2])
            strcpy (check_port[count], "drop");
    }

    fprintf (output, "%d\n", number_c);
    for (count = 0; count < number_c; count++)
        fprintf (output, "%c %c %s\n", check_IP[count][0], check_IP[count][1], check_port[count]);

    fclose(input);
    fclose(output);

    return (0);
}
