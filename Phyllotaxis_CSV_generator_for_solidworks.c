/*
creates a CSV file of (x, y) values of coordinates to a Phyllotaxis set
or solidworks macro
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define PI  3.14159265359
#define M2IPS 39.3700787


char *generate_filename_sw(int i);
char *generate_filename(int i);

int main()
{
    FILE *fp;
    FILE *fp_sw;
    long int timeSuffix = time(NULL);
    char *fileName  = generate_filename(timeSuffix);
    char *fileName_sw  = generate_filename_sw(timeSuffix);
    fp = fopen(fileName, "wb");
    fp_sw = fopen(fileName_sw, "wb");
    char output[64];// for csv line
    char output_sw[512]; //
    char pResultBuf[4096] = {0};
    char const bufPrefix[] = " Dim swApp As Object \r\n Dim Part As Object \r\n Dim boolstatus As Boolean \r\n Dim longstatus As Long, longwarnings As Long \r\n Sub main() \r\n Set swApp = Application.SldWorks \r\n Set Part = swApp.ActiveDoc \r\n boolstatus = Part.Extension.SelectByID2(\"Top Plane\", \"PLANE\", 0, 0, 0, False, 0, Nothing, 0) \r\n Dim skPoint As Object \r\n Part.SketchManager.InsertSketch True \r\n";
    memcpy(&pResultBuf[0], &bufPrefix[0],  strlen(bufPrefix));
    fprintf(fp_sw, "%s", pResultBuf);
    char const pointPrefix[] = "Set skPoint = Part.SketchManager.CreatePoint(";
    char const programSuffix[] = "Part.SketchManager.InsertSketch True \r\n Part.ClearSelection2 True \r\n End Sub";

    int n = 170;
    double c = 1;
    double a, r, x, y;
    double diameter_inches = 10;
    double diameter = (diameter_inches / M2IPS);
    double height = diameter, width = diameter;



    int i = 1;
    while(i <= n)
    {
        memset(&output[0], 0, sizeof(char)*(64));
        memset(&output_sw[0], 0, sizeof(char)*(512));

        a = (double)i * 137.508f;
        r = c * sqrt(i);
//        x = r * cos((a*PI)/180) + width / 2;       // width/2 and height/2 only needed for rendering in non-NDI screenspace, not SW
//        y = r * sin((a*PI)/180) + height / 2;      // width/2 and height/2 only needed for rendering in non-NDI screenspace, not SW
        x = r * cos((a*PI)/180);
        y = r * sin((a*PI)/180);
        x = x / M2IPS;
        y = y / M2IPS;

        snprintf(&output[0], 64, "%10f", x);
        output[10] = 0x2C;    // hex [,]
        output[11] = 0x20;    // hex space
        snprintf(&output[12], 20, "%10f", y);
        output[22] = '\n';
        printf("%s\n", &output[0]);
        fprintf(fp, "%s", &output[0]);

        fprintf(fp_sw, "%s", pointPrefix);

        snprintf(&output_sw[0], 64, "%10f", x);
        output_sw[10] = 0x2C;    // hex [,]
        output_sw[11] = 0x20;    // hex space
        snprintf(&output_sw[12], 20, "%10f", y);
        fprintf(fp_sw, "%s", &output_sw[0]);
        fprintf(fp_sw, "%s", ", 0#)\n"); // close the CreatePoint method

        i++;
    }
    fprintf(fp_sw, "%s", programSuffix);
    return 0;
}

char *generate_filename_sw(int i)
{
    char dest[64];
    char middle[64];
    static char filenamebuf[64];
    char suffix[] = ".txt";
    char *prefix = "SW_strainer_macro_";
    sprintf(middle, "%d", i);
    strcpy(dest, prefix);
    strcat(dest, middle);
    strcat(dest, suffix);
    strcpy(filenamebuf, dest);
    char *p_filenamebuf = &filenamebuf[0];
    return p_filenamebuf;
}

char *generate_filename(int i)
{
    char dest[64];
    char middle[64];
    static char filenamebuf[64];
    char suffix[] = ".csv";
    char *prefix = "pointMap_";
    sprintf(middle, "%d", i);
    strcpy(dest, prefix);
    strcat(dest, middle);
    strcat(dest, suffix);
    strcpy(filenamebuf, dest);
    char *p_filenamebuf = &filenamebuf[0];
    return p_filenamebuf;
}
