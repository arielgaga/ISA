// ISA.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TROPO_ALT 10972
#define MIN_ALT 0
#define MAX_TROPO 20000
#define MAX_ALT 25000
#define TROPO_TEMP -56.5
#define ERROR  -99999
#define FTTOMET 0.3048
#define SLOPE_M 0.0065

enum units_type
{
    m = 0,
    ft = 1 
};

void calc_temp(double gnd_temp, double alt, units_type units)
{
    double res;
    float coeff = ((units == ft) ? (FTTOMET) : (1));
    double alt_m = alt * coeff;
    /*calc the temp differently according to hight*/
    if((MIN_ALT <= alt_m) && (alt_m < TROPO_ALT))
        res = gnd_temp - SLOPE_M * alt_m ;
    else if (alt_m < MAX_ALT)
        res = TROPO_TEMP;
    else res = ERROR;

    if (units == m)
        printf("ground temp = %f[c], Alt =%f[m] restemp = %f(c) \n", gnd_temp, alt, res);
    else if (units == ft)
        printf("ground temp = %f[c], Alt =%f[ft] restemp = %f[c] \n", gnd_temp, alt, res);
          
}


void calcISA()
{
    FILE* file;
    const char* filename = "no_file";
    errno_t err;
    char line[256];
    char* temp;
    char* alt;
    double GND_temp_d;
    double alt_d;
    double res;
    units_type units;
    int idx = 0;
    int fileNumber = 0;

    printf("Hello! welcome to ISA App!\nPlease choose an input file, enter 1 ~ 3 to choose: ");
    while (true)
    {
        scanf_s("%d", &fileNumber);
        if ((0 < fileNumber) && (fileNumber <= 3))
            break;
        printf("invalid key Error! enter 1 ~ 3 to choose:");
    }

    if (fileNumber == 1)
        filename = "Test_1.txt";
    else if (fileNumber == 2)
        filename = "Test_2.txt";
    else if (fileNumber == 3)
        filename = "Test_3.txt";

    /*continue only if a file has been loaded*/
    if (filename != NULL)
    {
        err = fopen_s(&file, filename, "r");
        printf("reading filename %s\n", filename);

        /*iterates on the file lines*/
        while(fgets(line, sizeof(line), file))
        {
            /*splits temp and alt strings on each line*/
            temp = strtok_s(line, ",", &alt);

            /*read first line if idx == 0 in order to get the file units */
            if (idx == 0)
            {
                if ((strstr(alt, "Alt[m]") != NULL) || (strstr(alt, "Alt[c]") != NULL))
                {
                    printf("Alt data is in [m]: \n");
                    units = m;
                }
                else if (strstr(alt, "Alt[ft]") != NULL)
                {
                    printf("Alt data is in [ft]: \n");
                    units = ft;
                }
            }
            /*read the raw data*/
            else
            {
                GND_temp_d = atof(temp); //cast string to double
                alt_d = (atof(alt));
                calc_temp(GND_temp_d, alt_d, units);
            }
            idx++;
        }
    
        fclose(file);
    }
}


int main()
{
    calcISA();
    
}

