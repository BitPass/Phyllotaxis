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


char *generate_filename_sw(int i);
char *generate_filename(int i);

int get_holecount(int rungcount)
{
    int i, temp;
    int total = 0;
    for(i = 1; i <= rungcount; i++)
    {
        temp = 8*i;
        total = total + temp;
    }
    return total;
}

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
    char const bufPrefix[] = " Dim swApp As Object \r\n Dim Part As Object \r\n   \r\n Dim boolstatus As Boolean \r\n Dim longstatus As Long, longwarnings As Long \r\n Sub main() \r\n Set swApp = Application.SldWorks \r\n Set Part = swApp.ActiveDoc \r\n \r\n Part.UserFavors \r\n Part.SketchManager.AutoSolve = False \r\n Part.SketchManager.AutoInference = False \r\n \r\n boolstatus = Part.Extension.SelectByID2(\"Top Plane\", \"PLANE\", 0, 0, 0, False, 0, Nothing, 0) \r\n Dim skPoint As Object \r\n Part.SketchManager.InsertSketch True \r\n";
    memcpy(&pResultBuf[0], &bufPrefix[0],  strlen(bufPrefix));
    fprintf(fp_sw, "%s", pResultBuf);
    char const pointPrefix[] = "Set skPoint = Part.SketchManager.CreatePoint(";
    char const programSuffix[] = "Part.SketchManager.InsertSketch True \r\n Part.ClearSelection2 True \r\n End Sub";



/*
 Part.UserFavors
Part.SketchManager.AutoSolve = False
Part.SketchManager.AutoInference = False
*/


//    double c = 1/39.3700787;
    double a; // current angle
    int rung_max;
    int rung_curr;
    int p;	// points per current rung
    double slice_degs;
    double r;
    double radius_step;
    double radius_start;
    double s; // sweep between two points per current rung
    double scale = 39.3700787; // inches to Solidworks internal (metric)
    double x;
    double y;
	int i;

    radius_start = 1.10;
    radius_step = 1.10;
    rung_max = 10;


    for(rung_curr = 1; rung_curr <= rung_max; rung_curr++)
	{
		p = rung_curr * 8;
		s = 360.0 / p;
		i = 0;
		a = 0;
		for(i = 0; i < p; i++)
		{
			/*housekeeping*/
			memset(&output[0], 0, sizeof(char)*(64));
        	memset(&output_sw[0], 0, sizeof(char)*(512));

			a = i*s;
			r = radius_start + ((rung_curr-1)*radius_step);
	        x = r * cos((a*PI)/180);
	        y = r * sin((a*PI)/180);
	        x = x / scale;
	        y = y / scale;


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
	        fprintf(fp_sw, "%s", ", 0#)\r\n"); // close the CreatePoint method
		}
	}

    fprintf(fp_sw, "%s", programSuffix); // add closing syntax only to SW macro file

    printf("\n Holecount: %d", get_holecount(rung_max));

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



