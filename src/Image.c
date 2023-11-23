/**
 * @file Image.c
 * @brief Image treatment function implementation
 * @author Antony Madaleno
 * @version 1.0
 * @date 25-01-2023
 *
 *
 */


#include "Image.h"
#include <math.h>
#include "stdio.h"
#include "stdlib.h"
#include <pthread.h>

Image * Image_set(unsigned short height, unsigned short width)
{
    Image * img = (Image *) calloc(1, sizeof(Image));

    img->height = height;
    img->width = width;

    img->R = Matrix_generate(width, height);
    img->G = Matrix_generate(width, height);
    img->B = Matrix_generate(width, height);

    return img;
}

void Image_free(Image * img)
{
    Matrix_free(img->R);
    Matrix_free(img->G);
    Matrix_free(img->B);
}

Vec3 * Image_getPixel(Image * img, unsigned short x, unsigned short y)
{
    if(img == NULL)
        return NULL;

    Vec3 * output = (Vec3 *) calloc(1, sizeof(Vec3) );

    output->x = * Matrix_at(img->R, x, y);
    output->y = * Matrix_at(img->G, x, y);
    output->z = * Matrix_at(img->B, x, y);

    return output;
}

void Image_setPixel(Image * img, unsigned short x, unsigned short y, Vec3 * pixel)
{
    Matrix_setAt(img->R, x, y, pixel->x);
    Matrix_setAt(img->G, x, y, pixel->y);
    Matrix_setAt(img->B, x, y, pixel->z);
}

Image * Image_import(const char * filepath)
{
    //get data from a bmp file
    FILE * file;
    file = fopen(filepath, "rb");

    if (file == NULL)
    {
        printf("CAN'T OPEN : '%s'", filepath);
        return NULL;
    }

    if( file != NULL)
    {

        unsigned char * header = (unsigned char *) calloc(14, sizeof(unsigned char));
        if (!header) {
            fprintf(stderr, "Error: Memory allocation failed for header.\n");
            exit(EXIT_FAILURE);
        }
        unsigned char * info_header = (unsigned char *) calloc(40, sizeof(unsigned char));
        if (!info_header) {
            fprintf(stderr, "Error: Memory allocation failed for info_header.\n");
            exit(EXIT_FAILURE);
        }

        fread(header, sizeof(char), 14, file);
        fread(info_header, sizeof(char), 40, file);

        unsigned int width = 0;
        unsigned int height = 0;

        width += info_header[4]     << 0;
        width += info_header[5]     << 8;
        width += info_header[6]     << 16;
        width += info_header[7]     << 24;

        height += info_header[8]    << 0;
        height += info_header[9]    << 8;
        height += info_header[10]   << 16;
        height += info_header[11]   << 24;

        unsigned char * data = (unsigned char *) calloc(3, sizeof(unsigned char));
        if (!data) {
            fprintf(stderr, "Error: Memory allocation failed for buffer data.\n");
            exit(EXIT_FAILURE);
        }

        Image * img = Image_set(height, width);

        free(header);
        free(info_header);

        const unsigned char padding_amount = ( 4 - ( img->width * 3 ) % 4) % 4;

        for (unsigned short y = 0; y < height; y++)
        {
            for (unsigned short x = 0; x < width; x++)
            {
                fread(data, sizeof(unsigned char), 3, file);
                Vec3 * pixel = (Vec3 *) calloc(1, sizeof(Vec3) );
                Vec3_set(
                    pixel,
                    (float) data[2] / 255.0f ,
                    (float) data[1] / 255.0f ,
                    (float) data[0] / 255.0f 
                );
                Image_setPixel(img, x, y, pixel);
                free(pixel);
                if (x == width - 1)
                    fread(data, sizeof(unsigned char), padding_amount, file);
            }
        }

        free(data);
        return img;
    }

    return NULL;
}

void Image_export(Image * img, char * filepath)
{
    FILE * file;
    file = fopen(filepath, "wb");

    if( file != NULL)
    {
        unsigned char bmppad[1] = { 0 };

        const unsigned char padding_amount = ( 4 - ( img->width * 3 ) % 4) % 4;
        const unsigned char header_size = 14;
        const unsigned char info_header_size = 40;
        const unsigned int file_size = header_size + info_header_size + img->width * img->height * 3 + padding_amount * img->height;

        unsigned char file_header[ header_size ];

        //FILE TYPE
        file_header[0] = 'B';
        file_header[1] = 'M';

        //FILE SIZE
        file_header[2] = file_size;
        file_header[3] = file_size >> 8;
        file_header[4] = file_size >> 16;
        file_header[5] = file_size >> 24;

        //RESERVED 0 Not Used
        file_header[6] = 0;
        file_header[7] = 0;

        //RESERVED 1 Not Used
        file_header[8] = 0;
        file_header[9] = 0;

        //PIXEL DATA OFFSET
        file_header[10] = header_size + info_header_size;
        file_header[11] = 0;
        file_header[12] = 0;
        file_header[13] = 0;

        unsigned char information_header[ info_header_size ];

        //HEADER SIZE
        information_header[0] = info_header_size;
        information_header[1] = 0;
        information_header[2] = 0;
        information_header[3] = 0;

        //IMAGE WIDTH
        information_header[4] = img->width;
        information_header[5] = img->width >> 8;
        information_header[6] = img->width >> 16;
        information_header[7] = img->width >> 24;

        //IMAGE HEIGHT
        information_header[8]  = img->height;
        information_header[9]  = img->height >> 8;
        information_header[10] = img->height >> 16;
        information_header[11] = img->height >> 24;

        //PLANES
        information_header[12] = 1;
        information_header[13] = 0;

        //BITS PER PIXELS
        information_header[14] = 24;
        information_header[15] = 0;

        //COMPRESSION (NO COMPRESSION)
        information_header[16] = 0;
        information_header[17] = 0;
        information_header[18] = 0;
        information_header[19] = 0;

        //IMAGE SIZE (NO COMPRESSION)
        information_header[20] = 0;
        information_header[21] = 0;
        information_header[22] = 0;
        information_header[23] = 0;

        //X PIXEL PER METER
        information_header[24] = 0;
        information_header[25] = 0;
        information_header[26] = 0;
        information_header[27] = 0;

        //Y PIXEL PER METER
        information_header[28] = 0;
        information_header[29] = 0;
        information_header[30] = 0;
        information_header[31] = 0;

        //TOTAL COLORS
        information_header[32] = 0;
        information_header[33] = 0;
        information_header[34] = 0;
        information_header[35] = 0;

        //IMPORTANT COLORS [IGNORED]
        information_header[36] = 0;
        information_header[37] = 0;
        information_header[38] = 0;
        information_header[39] = 0;

        fwrite((char *) file_header, header_size, 1, file);
        fwrite((char *) information_header, info_header_size, 1, file);

        for (unsigned short y = 0; y < img->height; y++)
        {
            for (unsigned short x = 0; x < img->width; x++)
            {
                Vec3 * pixel;
                pixel = Image_getPixel(img, x, y);
                unsigned char r = (unsigned char) ( pixel->x * 255.0f );
                unsigned char g = (unsigned char) ( pixel->y * 255.0f );
                unsigned char b = (unsigned char) ( pixel->z * 255.0f );

                unsigned char color[3] = { b, g, r };
                fwrite((char *) color, sizeof(char) * 3, 1, file);
            }

            for (unsigned char p = 0; p < padding_amount; p++)
                fwrite( (char *) bmppad, sizeof(char), 1, file);
        }

    }
    else
        printf("can't open file %s", filepath);

    fclose(file);
}

//apply mat to all channels
Image * Image_applyMatrix(Image * img, Matrix * mat)
{
    if (img == NULL || mat == 0 || mat->n_cols % 2 != 1 || mat->n_cols != mat->n_rows)
        return NULL;

    Image * out = Image_set(img->height, img->width);
    short offset = (mat->n_rows -1)/2;

    for (unsigned short y = 0; y < img->height; y++)
    {
        for (unsigned short x = 0; x < img->width; x++)
        {
            if ( x < offset || y < offset || x > img->width -offset -1 || y > img->height -offset -1 )
            {
                Vec3 * pix = Image_getPixel(img, x, y);
                Image_setPixel( out, x, y, pix );
                free(pix);
            }
            else
            {
                Vec3 * pix = (Vec3 *) calloc( 1, sizeof(Vec3) );

                for (unsigned short r = 0; r < mat->n_rows; r++)
                {
                    for (unsigned short c = 0; c < mat->n_cols; c++)
                    {
                        Vec3 * tmp = Image_getPixel( img, x + c - offset, y + r - offset);
                        pix->x += ( * Matrix_at(mat, c, r) ) * tmp->x;
                        pix->y += ( * Matrix_at(mat, c, r) ) * tmp->y;
                        pix->z += ( * Matrix_at(mat, c, r) ) * tmp->z;
                        free(tmp);
                    }
                }

                Image_setPixel(out, x, y, pix);
                free(pix);
            }
        }
    }

    return out;

}

void Image_applyTreshold(Image * img, Vec3 * v)
{
    if (img == NULL || v == NULL)
        return;

    for (unsigned short y = 0; y < img->height; y++)
    {
        for (unsigned short x = 0; x < img->width; x++)
        {
            Vec3 * pix = Image_getPixel(img, x, y);

            if (pix->x < v->x) pix->x = 0; else pix->x = 1; 
            if (pix->y < v->y) pix->y = 0; else pix->y = 1; 
            if (pix->z < v->z) pix->z = 0; else pix->z = 1;

            Image_setPixel(img, x, y, pix);

            free(pix);
        }
    }
}

Image * Image_medianFilter(Image * img, unsigned short s)
{
    if (img == NULL || s % 2 != 1)
        return NULL;

    Image * out = Image_set(img->height, img->width);
    short offset = (s-1)/2;

    for (unsigned short y = 0; y < img->height; y++)
    {
        for (unsigned short x = 0; x < img->width; x++)
        {
            if ( x < offset || y < offset || x > img->width -offset -1 || y > img->height -offset -1 )
            {
                Vec3 * pix = Image_getPixel(img, x, y);
                Image_setPixel( out, x, y, pix );
                free(pix);
            }
            else
            {
                Vec3 * pix = (Vec3 *) calloc( 1, sizeof(Vec3) );

                float * r_channel = (float *) calloc( s*s, sizeof(float) );
                float * g_channel = (float *) calloc( s*s, sizeof(float) );
                float * b_channel = (float *) calloc( s*s, sizeof(float) );

                for (unsigned short r = 0; r < s; r++)
                {
                    for (unsigned short c = 0; c < s; c++)
                    {
                        Vec3 * vec = Image_getPixel( img, x + c - offset, y + r - offset);
                        r_channel[c + r*s] = vec->x;
                        g_channel[c + r*s] = vec->y;
                        b_channel[c + r*s] = vec->z;
                        free(vec);
                    }
                }

                float f;
                for (unsigned short i = 0; i < s*s -1; i++)
                    for (unsigned short j = i + 1; j < s*s; j++)
                    {
                        if (r_channel[i] > r_channel[j])
                        {
                            f = r_channel[i];
                            r_channel[i] = r_channel[j];
                            r_channel[j] = f;
                        }

                        if (g_channel[i] > g_channel[j])
                        {
                            f = g_channel[i];
                            g_channel[i] = g_channel[j];
                            g_channel[j] = f;
                        }

                        if (b_channel[i] > b_channel[j])
                        {
                            f = b_channel[i];
                            b_channel[i] = b_channel[j];
                            b_channel[j] = f;
                        }
                    }

                unsigned short med = (s*s - 1)/2;
                Vec3_set(pix, r_channel[med], g_channel[med], b_channel[med]);
                Image_setPixel(out, x, y, pix);
                free(pix);
            }
        }
    }

    return out;

}

Image * Image_histoCumulatifBMP(Image * img, unsigned short w, unsigned short h)
{
    unsigned int * buf_R = (unsigned int *) calloc( 256, sizeof(unsigned int) );
    unsigned int * buf_G = (unsigned int *) calloc( 256, sizeof(unsigned int) );
    unsigned int * buf_B = (unsigned int *) calloc( 256, sizeof(unsigned int) );

    w = w - w%256;
    unsigned char step = w / 256;

    Image * histo = Image_set(h,w);

    for (unsigned short i = 0; i < img->width; i++)
        for (unsigned short j = 0; j < img->height; j++)
        {
            buf_R[ (unsigned char) ( 255*(* Matrix_at(img->R, i ,j)) ) ]++;
            buf_G[ (unsigned char) ( 255*(* Matrix_at(img->G, i ,j)) ) ]++;
            buf_B[ (unsigned char) ( 255*(* Matrix_at(img->B, i ,j)) ) ]++;
        }

    for (unsigned short i = 1; i < 255; i++)
    {
        buf_R[i] += buf_R[i-1];
        buf_G[i] += buf_G[i-1];
        buf_B[i] += buf_B[i-1];
    }
        

    unsigned int max = 0; 
    for ( unsigned char i = 0; i < 255; i++ )
    {
        if (max < buf_R[i])
            max = buf_R[i];
        if (max < buf_G[i])
            max = buf_G[i];
        if (max < buf_B[i])
            max = buf_B[i];
    }

    for ( unsigned char i = 0; i < 255; i++ )
        for (unsigned char x = 0; x < step; x++)
        {
            for (unsigned short r = 0; r < (h-1)*buf_R[i]/max; r++)
                Matrix_setAt(histo->R, i * step + x, r, 1.0);
            for (unsigned short g = 0; g < (h-1)*buf_G[i]/max; g++)
                Matrix_setAt(histo->G, i * step + x, g, 1.0);
            for (unsigned short b = 0; b < (h-1)*buf_B[i]/max; b++)
                Matrix_setAt(histo->B, i * step + x, b, 1.0);
        }

    free(buf_R);
    free(buf_G);
    free(buf_B);

    return histo;
}

Image * Image_histoBMP(Image * img, unsigned short w, unsigned short h)
{
    unsigned int * buf_R = (unsigned int *) calloc( 256, sizeof(unsigned int) );
    unsigned int * buf_G = (unsigned int *) calloc( 256, sizeof(unsigned int) );
    unsigned int * buf_B = (unsigned int *) calloc( 256, sizeof(unsigned int) );

    w = w - w%256;
    unsigned char step = w / 256;

    Image * histo = Image_set(h,w);

    for (unsigned short i = 0; i < img->width; i++)
        for (unsigned short j = 0; j < img->height; j++)
        {
            buf_R[ (unsigned char) ( 255*(* Matrix_at(img->R, i ,j)) ) ]++;
            buf_G[ (unsigned char) ( 255*(* Matrix_at(img->G, i ,j)) ) ]++;
            buf_B[ (unsigned char) ( 255*(* Matrix_at(img->B, i ,j)) ) ]++;
        }

    unsigned int max = 0; 
    for ( unsigned char i = 0; i < 255; i++ )
    {
        if (max < buf_R[i])
            max = buf_R[i];
        if (max < buf_G[i])
            max = buf_G[i];
        if (max < buf_B[i])
            max = buf_B[i];
    }

    for ( unsigned char i = 0; i < 255; i++ )
        for (unsigned char x = 0; x < step; x++)
        {
            for (unsigned short r = 0; r < (h-1)*buf_R[i]/max; r++)
                Matrix_setAt(histo->R, i * step + x, r, 1.0);
            for (unsigned short g = 0; g < (h-1)*buf_G[i]/max; g++)
                Matrix_setAt(histo->G, i * step + x, g, 1.0);
            for (unsigned short b = 0; b < (h-1)*buf_B[i]/max; b++)
                Matrix_setAt(histo->B, i * step + x, b, 1.0);
        }

    free(buf_R);
    free(buf_G);
    free(buf_B);

    return histo;
}

unsigned int ** Image_histo(Image * img)
{
    unsigned int * buf_R = (unsigned int *) calloc( 256, sizeof(unsigned int) );
    unsigned int * buf_G = (unsigned int *) calloc( 256, sizeof(unsigned int) );
    unsigned int * buf_B = (unsigned int *) calloc( 256, sizeof(unsigned int) );

    for (unsigned short i = 0; i < img->width; i++)
        for (unsigned short j = 0; j < img->height; j++)
        {
            buf_R[ (unsigned char) ( 255*(* Matrix_at(img->R, i ,j)) ) ]++;
            buf_G[ (unsigned char) ( 255*(* Matrix_at(img->G, i ,j)) ) ]++;
            buf_B[ (unsigned char) ( 255*(* Matrix_at(img->B, i ,j)) ) ]++;
        }

    unsigned int ** values = (unsigned int **) calloc(3, sizeof(unsigned int) );
    values[0] = buf_R;
    values[1] = buf_G;
    values[2] = buf_B;

    return values;
}

unsigned int ** Image_histoCumulatif(Image * img)
{
    unsigned int * buf_R = (unsigned int *) calloc( 256, sizeof(unsigned int) );
    unsigned int * buf_G = (unsigned int *) calloc( 256, sizeof(unsigned int) );
    unsigned int * buf_B = (unsigned int *) calloc( 256, sizeof(unsigned int) );

    for (unsigned short i = 0; i < img->width; i++)
        for (unsigned short j = 0; j < img->height; j++)
        {
            buf_R[ (unsigned char) floorf( 256*(* Matrix_at(img->R, i ,j)) ) ]++;
            buf_G[ (unsigned char) floorf( 256*(* Matrix_at(img->G, i ,j)) ) ]++;
            buf_B[ (unsigned char) floorf( 256*(* Matrix_at(img->B, i ,j)) ) ]++;
        }

    for (unsigned short i = 1; i < 256; i++)
    {
        buf_R[i] += buf_R[i-1];
        buf_G[i] += buf_G[i-1];
        buf_B[i] += buf_B[i-1];
    }
        

    unsigned int max = 0; 
    for ( unsigned short i = 0; i < 256; i++ )
    {
        if (max < buf_R[i])
            max = buf_R[i];
        if (max < buf_G[i])
            max = buf_G[i];
        if (max < buf_B[i])
            max = buf_B[i];
    }

    unsigned int ** values = (unsigned int **) calloc(3, sizeof(unsigned int) );
    values[0] = buf_R;
    values[1] = buf_G;
    values[2] = buf_B;

    return values;
}

Image * Image_extend(Image * img)
{
    Image * ex = Image_set(img->height, img->width);
    float max = fmax(fmax( Matrix_max(img->R) , Matrix_max(img->G) ), Matrix_max(img->B) );
    float min = fmin(fmin( Matrix_min(img->R) , Matrix_min(img->G) ), Matrix_min(img->B) );

    for (unsigned short x = 0; x < img->width; x++)
        for (unsigned short y = 0; y < img->height; y++)
        {
            Matrix_setAt(ex->R, x, y, ( (* Matrix_at(img->R, x, y)) - min ) * 1.0 / (max - min) );
            Matrix_setAt(ex->G, x, y, ( (* Matrix_at(img->G, x, y)) - min ) * 1.0 / (max - min) );
            Matrix_setAt(ex->B, x, y, ( (* Matrix_at(img->B, x, y)) - min ) * 1.0 / (max - min) );
        }

    return ex;
}

Image * Image_applyLUTS(Image * img, float * lut_R, float * lut_G, float * lut_B)
{

    Image * res = Image_set(img->height, img->width);

    for (unsigned short i = 0; i < img->width; i++)
        for (unsigned short j = 0; j < img->height; j++)
        {
            Matrix_setAt( res->R, i, j, lut_R[ (unsigned char) (255 * ( * Matrix_at(img->R, i, j) ) )] );
            Matrix_setAt( res->G, i, j, lut_G[ (unsigned char) (255 * ( * Matrix_at(img->G, i, j) ) )] );
            Matrix_setAt( res->B, i, j, lut_B[ (unsigned char) (255 * ( * Matrix_at(img->B, i, j) ) )] );
        }

    return res;
}

Image * Image_equalize(Image * img)
{
    Image * res = Image_set(img->height, img->width);

    unsigned int ** H = Image_histoCumulatif(img);
    unsigned int * buffer = (unsigned int *) calloc( 256, sizeof(unsigned int) );

    for (unsigned char i = 0; i < 255; i++)
        buffer[i] = H[0][i] + H[1][i] + H[2][i];

    free(H[0]);
    free(H[1]);
    free(H[2]);
    free(H);

    unsigned int pixCount = img->height * img->width * 3;

    for (unsigned short i = 0; i < img->width; i++)
    {
        for (unsigned short j = 0; j < img->height; j++)
        {
            Matrix_setAt( res->R, i, j, (float) buffer[ (unsigned char) (255 * ( * Matrix_at(img->R, i, j) ) )] / pixCount  );
            Matrix_setAt( res->G, i, j, (float) buffer[ (unsigned char) (255 * ( * Matrix_at(img->G, i, j) ) )] / pixCount  );
            Matrix_setAt( res->B, i, j, (float) buffer[ (unsigned char) (255 * ( * Matrix_at(img->B, i, j) ) )] / pixCount  );
        }  
    }

    return res;
}

Image * Image_specHisto(Image * img, unsigned int * spec_R, unsigned int * spec_G, unsigned int * spec_B)
{
    
    unsigned int ** X = Image_histoCumulatif(img);
    unsigned int ** S = (unsigned int **) calloc( 3, sizeof(unsigned int *) );
    unsigned int ** H = (unsigned int **) calloc( 3, sizeof(unsigned int *) );

    S[0] = spec_R;
    S[1] = spec_G;
    S[2] = spec_B;

    H[0] = (unsigned int *) calloc( 256, sizeof(unsigned int) );
    H[1] = (unsigned int *) calloc( 256, sizeof(unsigned int) );;
    H[2] = (unsigned int *) calloc( 256, sizeof(unsigned int) );;

    float ** luts = (float **) calloc( 3, sizeof(float *) );

    for (unsigned char c = 0; c < 3; c++)
        for (unsigned short i = 0; i < 256; i++)
        {
            H[c][255-i] = (unsigned int) 255 - floorf( (float) 255.0 * X[c][i] / X[c][255] );
            S[c][i] = (unsigned int) floorf( (float) 255.0 * S[c][i] / S[c][255] );
        }

    for (unsigned char c = 0; c < 3; c++)
    {
        luts[c] = (float *) calloc( 256, sizeof(float) );
        for (unsigned short i = 0; i < 256; i++)
            luts[c][i] = H[c][ S[c][i] ] / 255.0;
    }

    Image * res = Image_applyLUTS(img, luts[0], luts[1], luts[2]);
    
    return res;
}

Image * Image_toGray(Image * img)
{
    Image * res = Image_set(img->height, img->width);
    Matrix_free(res->G);
    Matrix_free(res->B);

    res->G = res->R;
    res->B = res->R;

    for (unsigned short y = 0; y < img->height; y++)
        for (unsigned short x = 0; x < img->width; x++)
        {
            float v =   0.299 * ( * Matrix_at(img->R, x, y) );
            v +=        0.587 * ( * Matrix_at(img->G, x, y) );
            v +=        0.114 * ( * Matrix_at(img->B, x, y) );
            
            if (v <= 0.0031308)
                v *= 5;
            else
                v = 1.015 * powf(v, 0.75) - 0.015;
            
            Matrix_setAt( res->R, x, y, v );
        }

    return res;
}

typedef struct thread_data {
    Matrix * channel;
    unsigned char * data;
    unsigned char c;
} thread_data;

static void * thread_toArray(void * args)
{
    thread_data * th_d = (thread_data *) args;
    unsigned char * data = th_d->data;
    unsigned char c = th_d->c;
    Matrix * channel = th_d->channel;

    for (unsigned short i = 0; i < channel->n_cols; i++)
        for (unsigned short j = 0; j < channel->n_rows; j++)
            data[(i * channel->n_rows + j) * 3 + c] = (unsigned char) (*Matrix_at(channel, i, j) * 255.f);

    return NULL;
}

unsigned char * Image_toArray(Image * img)
{
    unsigned char * data = (unsigned char *) calloc(img->width * img->height * 3, sizeof(unsigned char));
    if (!data) {
        fprintf(stderr, "Error: Memory allocation failed for buffer data.\n");
        exit(EXIT_FAILURE);
    }

    thread_data * th_d = (thread_data *) calloc(3, sizeof(thread_data));
    if (!th_d) {
        fprintf(stderr, "Error: Memory allocation failed for threads data.\n");
        exit(EXIT_FAILURE);
    }
    th_d[0].channel = img->R;
    th_d[0].c = 0;
    th_d[0].data = data;

    th_d[1].channel = img->G;
    th_d[1].c = 1;
    th_d[1].data = data;

    th_d[2].channel = img->B;
    th_d[2].c = 2;
    th_d[2].data = data;

    pthread_t * threads = (pthread_t *) calloc(3, sizeof(pthread_t));
    if (!threads) {
        fprintf(stderr, "Error: Memory allocation failed for threads.\n");
        exit(EXIT_FAILURE);
    }
    unsigned char i;

    for (i = 0; i < 3; i++)
        pthread_create(&threads[i], NULL, thread_toArray, (void *) &th_d[i]);

    for (i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    free(threads);
    free(th_d);

    return data;
}