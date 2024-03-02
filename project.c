#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>

#define name "Ilonczai András"
#define version 1.01
#define date "2024.03.02"

#define _bitsperpixel 24
#define _planes 1
#define _compression 0
//#define _pixelbytesize _height*_width*_bitsperpixel/8
//#define _filesize _pixelbytesize+sizeof(bitmap)
#define _xpixelpermeter 3937 //2835 , 72 DPI
#define _ypixelpermeter 3937 //2835 , 72 DPI
#define pixel 0xFF
#pragma pack(push,1)

typedef struct
{
    uint8_t signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader;

typedef struct
{
    uint32_t dibheadersize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
} bitmapinfoheader;

typedef struct 
{
    fileheader fileheader;
    bitmapinfoheader bitmapinfoheader;
} bitmap;
#pragma pack(pop)

int Measurement(int **Values)
{
    time_t T1; // Variable to store current time

    // Get current time and store it in T1
    time(&T1);

    struct tm *T4;
    T4 = localtime(&T1); // Convert current time to local time and store in T4

    //1. 35,4838% == 11/31 kovetkezo elemet csokkentjuk 1-el
    //2. 42,8571% kovetkezo elemet noveljuk 1-el
    //3. 21,6591% a kovetkezo elem marad ugyanaz
    
    int perc = T4->tm_min;
    int ms = T4->tm_sec;

    // int perc = 15;
    // int ms = 24;

    // int perc = 41;
    // int ms = 27;

    int eredmeny = 0;

    if (perc >= 0 && perc < 15)
    {   
        if (perc == 0)
        {
            eredmeny = 100;
        }
        else
        {
            eredmeny = perc * 60 + ms;
        }
    }

    if(perc >= 15 && perc < 30)
    {   
        if (perc == 15)
        {
            eredmeny = 100;
        }
        else
        {   
            perc -= 15;
            eredmeny = perc * 60 + ms;
        }
    }

    if (perc >= 30 && perc < 45)
    {   
        if (perc == 30)
        {
            eredmeny = 100;
        }
        else
        {
            perc -= 30;
            eredmeny = perc * 60 + ms;
        }
    }
    
    if(perc >= 45 && perc < 60)
    {
        if (perc == 45)
        {
            eredmeny = 100;
        }
        else
        {
            perc -= 45;
            eredmeny = perc * 60 + ms;
        }
    }

    srand(time(NULL));

    int *values = (int *)malloc(eredmeny * sizeof(int));
    values[0] = 0;

    for (int i = 1; i < eredmeny; i++)
    {   
        double d = (double)rand() / RAND_MAX;

        if (d < 0.428571)
        {
            values[i] = values[i - 1] + 1;
        }
        else if (d >= 0.428571 && d < (0.354838 + 0.428571))
        {
            values[i] = values[i - 1] - 1;
        }
        else
        {
            values[i] = values[i - 1];
        }
    }

    *Values = values;

    return eredmeny;
}

void help()
{
    printf("Lehetseges parancsok:\n");
    printf("--help\n");
    printf("--version   verzio lekerdezes\n");

    printf("\nProgram uzemmodjai:\n");
    printf("-send       kuldo uzemmod\n");
    printf("-receive    fogado uzemmod\n");

    printf("\nKommunikacio uzemmodjai:\n");
    printf("-file       file uzemmod\n");
    printf("-socket     socket uzemmod\n");
}

void vers()
{
    printf("Developer: %s", name);
    printf("\nVersion: %.1f", version);
    printf("\nDate: %s\n", date);
}

void BMPcreator(int *Values, int numvalues)
{   
    int pixelbytesize = (numvalues*numvalues*24)/8;
    int filesize = pixelbytesize+sizeof(bitmap);
    
    FILE *fp = fopen("chart.bmp","wb");
    bitmap *pbitmap  = (bitmap*)calloc(1,sizeof(bitmap));
    uint8_t *pixelbuffer = (uint8_t*)malloc(pixelbytesize);

    strcpy(pbitmap->fileheader.signature,"BM");
    pbitmap->fileheader.filesize = filesize;
    pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);
    pbitmap->bitmapinfoheader.dibheadersize =sizeof(bitmapinfoheader);
    pbitmap->bitmapinfoheader.width = numvalues;
    pbitmap->bitmapinfoheader.height = numvalues;
    pbitmap->bitmapinfoheader.planes = _planes;
    pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
    pbitmap->bitmapinfoheader.compression = _compression;
    pbitmap->bitmapinfoheader.imagesize = pixelbytesize;
    pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter ;
    pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter ;
    pbitmap->bitmapinfoheader.numcolorspallette = 0;


    memset(pixelbuffer, 0xFF, pixelbytesize);

    // Draw the first pixel
    int y = numvalues / 2;
    int index = (y * numvalues + 0) * 3; // 3 bytes per pixel (24 bits)
    pixelbuffer[index] = 0xFF;             // Red component
    pixelbuffer[index + 1] = 0x00;         // Green component
    pixelbuffer[index + 2] = 0x00;         // Blue component

    for (int x = 1; x < numvalues; x++) 
    {
        int index = (y * numvalues + x) * 3;
        if (Values[x] == Values[x - 1]) 
        {

            pixelbuffer[index] = 0xFF;     // Red component
            pixelbuffer[index + 1] = 0x00; // Green component
            pixelbuffer[index + 2] = 0x00; // Blue component
        } 
        else if (Values[x] > Values[x - 1]) 
        {

            y++;
            index = (y * numvalues + x) * 3;
            pixelbuffer[index] = 0xFF;     // Red component
            pixelbuffer[index + 1] = 0x00; // Green component
            pixelbuffer[index + 2] = 0x00; // Blue component
        } 
        else 
        {
            y--;
            index = (y * numvalues + x) * 3;
            pixelbuffer[index] = 0xFF;     // Red component
            pixelbuffer[index + 1] = 0x00; // Green component
            pixelbuffer[index + 2] = 0x00; // Blue component
        }
    }

    fwrite(pbitmap, sizeof(bitmap), 1, fp);
    fwrite(pixelbuffer, pixelbytesize,1 , fp);
    fclose(fp);
    free(pbitmap);
    free(pixelbuffer);
}

int main (int argc, char *argv[]) 
{   
    int send = 1;
    int receive = 0;

    int file = 1;
    int socket = 0;
    if (argc < 2) 
    {
        printf("Tul keves argumentum");
        exit(1);
    }

    if (strstr(argv[0], "chart") != NULL) 
    {

    }

    //version
    if (strcmp(argv[1], "--version") == 0)
    {
        vers();
    }
    //help
    else if(strcmp(argv[1], "--help") == 0)
    {
        help();
    }
    else if ((strcmp(argv[1], "-receive") == 0))
    {
        send = 0;
        receive = 1;
    }
    else if ((strcmp(argv[1], "-socket") == 0) )
    {
        file = 0;
        socket = 1;
    }
    else if (strcmp(argv[1], "-send") == 0)
    {
        int *eredmeny;
        int res = Measurement(&eredmeny);
        printf("%d\n", res);
        BMPcreator(eredmeny, res);
        /*for (int i = 0; i < res; i++)
        {
            printf("%d\n", eredmeny[i]);
        }*/
    }
    else
    {
        help();
    }
    

    return 0;
}