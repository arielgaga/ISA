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
#define EROR  -0
#define FTTOMET 0.3048
#define M_COEFF 0.0065
#define FT_COEFF 0.0019812

enum units_type
{
    m = 0,
    ft = 1 
};

float calc_temp(double gnd_temp,double alt_m, units_type units)
{

    double temp;
    float coeff = ((units == ft) ? (FT_COEFF) : (M_COEFF));

    //////// 0 < h < 10 972.8
    if((MIN_ALT <= alt_m) && (alt_m < TROPO_ALT))
        temp = gnd_temp - coeff*alt_m ;

    else if (alt_m < MAX_ALT)
        temp = TROPO_TEMP;
    else temp = EROR;
          
    return temp;
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

        while (fgets(line, sizeof(line), file))
        {
            /* note that fgets don't strip the terminating \n, checking its
               presence would allow to handle lines longer that sizeof(line) */
            temp = strtok_s(line, ",", &alt);

            /*skip first line*/
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
            else
            {
                GND_temp_d = atof(temp);
                alt_d = (atof(alt));
                res = calc_temp(GND_temp_d, alt_d, units);
                if (units == m)
                {
                    printf("temp = %f[c], Alt =%f[m] restemp = %f(c) \n", GND_temp_d, alt_d, res);
                }
                else if (units == ft)
                {
                    printf("temp = %f[c], Alt =%f[ft] restemp = %f[c] \n", GND_temp_d, alt_d, res);
                }
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

