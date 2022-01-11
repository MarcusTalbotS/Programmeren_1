#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include <windows.h>

/**
 * Main for the Final Assignment of Programmeren 1.
 *
 * NB: Function loadImageFromFile is currently unused, because I have implemented the readGreyscaleBitmap-function.
 *
 * @author Marcus Talbot (1041464)
 * @return Error-code or 0.
 */
int main() {

    // Holds the data from the lena512.bmp file.
    unsigned char *fileData = readGreyscaleBitmap("..\\resources\\lena512.bmp");
    // X and Y dimension of the Lena-image.
    int sizeOfLena = 512;
    int lenaArray[sizeOfLena][sizeOfLena];

    // The bitmap data is upside-down and reversed, so we copy the data over to the lenaArray correctly.
    int rowInArray = 0;
    int colInArray = 511;
    for (int index = sizeOfLena * sizeOfLena - 1; index >= 0; index--) {
        lenaArray[rowInArray][colInArray] = fileData[index];
        colInArray--;
        if (colInArray == -1) {
            colInArray = 511;
            rowInArray++;
        }
    }

    // Freeing the memory as soon as possible.
    free(fileData);

    // Calculate and print the average grey-value of the full image, as per the assignment.
    int averageGreyValueLenaArray = calculateAverageGreyValue(sizeOfLena, sizeOfLena, lenaArray);
    printf("Average grey per pixel: %d\n", averageGreyValueLenaArray);

    int sizeSubset = 300;
    // Initialise the array that will hold the subset of lenaArray.
    int subset[sizeSubset][sizeSubset];

    // Offset. Used to determine where in the Lena-array we should start copying the data. Used 112 in testing.
    int offset = 112;

    // Fill the subset-array with the appropriate values.
    for (int row = 0; row < sizeSubset; row++) {
        for (int column = 0; column < sizeSubset; column++) {
            subset[row][column] = lenaArray[row + offset][column + offset];
        }
    }

    // Calculate and print the average grey-value of the subset, as per the assignment.
    int greyValueSmallImage = calculateAverageGreyValue(sizeSubset, sizeSubset, subset);
    printf("Average grey value per pixel in subset: %d\n\n", greyValueSmallImage);

    // Create a two-dimensional array of characters for the full image and the subset, so that we can print an entire
    // line of characters at a time. This saves quite a few seconds, as printing to the console is rather slow.
    char **lenaCharArray = greyValuesToChars(sizeOfLena, sizeOfLena, lenaArray);

    // Print the ASCII-image of the Lena to the console. Need to zoom out quite a bit to actually see it.
    printASCIIImage(sizeOfLena, lenaCharArray);

    for (int row = 0; row < sizeOfLena; row++) {
        free(lenaCharArray[row]);
    }
    free(lenaCharArray);

    char **smallLenaCharArray = greyValuesToChars(sizeSubset, sizeSubset, subset);

    printASCIIImage(sizeSubset, smallLenaCharArray);

    // Path to the Lena_klein.txt
    const char *smallImagePath = "..\\resources\\Lena_klein.txt";

    // Writing the subset-characters to a file, by specifying the amount of rows, and writing each individual row of
    // characters to the file, each followed by a linebreak. This function returns an integer, that can be used to figure
    // out whether the function has succeeded.
    int fileWriteFailed = writeImageToFile(smallImagePath, sizeSubset, smallLenaCharArray);
    // Here the value returned by the function is used to determine whether the function executed successfully.
    if (fileWriteFailed) {
        puts("Could not write the Lena_klein.txt");
    }

    // By specifying the amount of rows, we can easily iterate through the entire array, using strrev to reverse each
    // row before printing it.
    printReverseASCIIImage(sizeSubset, smallLenaCharArray);

    for (int row = 0; row < sizeSubset; row++) {
        free(smallLenaCharArray[row]);
    }
    free(smallLenaCharArray);

    return 0;
}

void loadImageFromFile(const char *filePath, int size, int imageArray[size][size]) {

    // If the filePath is NULL, we abort the program as there is no point in continuing. I've made the decision to
    // include this functionality, despite the fact that I know the path is not NULL, for the simple reason that, if
    // this were a different program without a hard-coded path, it might be NULL.
    if (filePath == NULL) {
        puts("File-path is NULL. Aborting...");
        exit(1);
    }

    FILE *fptr = NULL;

    // If the file cannot be opened, for whatever reason, we abort the program.
    if ((fptr = fopen(filePath, "r")) == NULL) {
        printf("Could not open file at file-path: %s\n\nPlease check the entered file-path.", filePath);
        exit(1);
    }

    char line[32];

    int row, column, greyValue;

    // While the file has more lines, we read every line.
    while (fgets(line, sizeof(line), fptr) != NULL) {
        // Each line should contain three integer; the row (1-indexed), the column (1-indexed), and the grey-value
        // (probably not 1-indexed). We assign each value to their appropriate variable, using each respective
        // variable's address.
        sscanf(line, "%d\t%d\t%d\n", &row, &column, &greyValue);

        // Decrement the row and column variables, because arrays start at 0.
        row--;
        column--;

        imageArray[row][column] = greyValue;
    }

    fclose(fptr);
}

int calculateAverageGreyValue(int rows, int columns, int imageArray[rows][columns]) {

    long sumOfValues = 0;

    // Simple add all the values stored in the given array.
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            sumOfValues += imageArray[row][column];
        }
    }

    // Divide the sum of all values by the total number of values in the array, to get the average value.
    return sumOfValues / (rows * columns);
}

void printASCIIImage(int size, char **imageArray) {

    // Printing per row, rather than per character is quite a bit faster, so we do that.
    for (int row = 0; row < size; row++) {
        printf("%s\n", imageArray[row]);
    }
}

void printReverseASCIIImage(int size, char **imageArray) {

    for (int row = 0; row < size; row++) {
        char *line = strrev(imageArray[row]);
        printf("%s\n", line);
    }
}

char **greyValuesToChars(int rows, int columns, int imageArray[rows][columns]) {
    char **asciiImage = malloc(rows * sizeof(char *));
    for (int row = 0; row < rows; row++) {
        asciiImage[row] = malloc((columns + 1) * sizeof(char)); // +1 for null-terminator
    }
    for (int row = 0; row < rows; row++) {
        asciiImage[row][columns] = '\0';
    }
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            // We divide the grey-value at any given point, and cast it to int to get the corresponding character.
            // 3.65 was chosen because 255 / 3.65 falls between 69 and 70. The last character in the charset is at index
            // 69, so a simple cast to int makes every character available.
            asciiImage[row][column] = charset[(int) (imageArray[row][column] / 3.65)];
        }
    }
    return asciiImage;
}

int writeImageToFile(const char *filePath, int rows, char **imageArray) {
    FILE *fptr;
    // Here we simply return 1 if the encounters a problem opening the specified file.
    if ((fptr = fopen(filePath, "w")) == NULL) {
        printf("Could not open file at %s\n", filePath);
        return 1;
    }
    // Write each row to the file, followed by a linebreak.
    for (int row = 0; row < rows; row++) {
        fprintf(fptr, "%s\n", imageArray[row]);
    }
    fclose(fptr);
    return 0;
}

unsigned char *readGreyscaleBitmap(const char *filePath) {
    FILE *filePtr;
    // Open the file in read-binary mode, as this is not a text-file.
    if ((filePtr = fopen(filePath,"rb")) == NULL) {
        fclose(filePtr);
        exit(2);
    }
    BITMAPFILEHEADER bitmapFileHeader;
    // Read the BITMAPFILEHEADER
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);
    // Check that the file is in fact a BMP-file.
    if (bitmapFileHeader.bfType != 0x4D42)
    {
        fclose(filePtr);
        exit(3);
    }
    BITMAPINFOHEADER bitmapInfoHeader;
    // Read the BITMAPINFOHEADER.
    fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr);
    // Move the file-pointer to the beginning of the data.
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    // Will hold the data from the bitmap.
    unsigned char *bitmapImage = (unsigned char*)malloc(bitmapInfoHeader.biSizeImage);
    fread(bitmapImage,bitmapInfoHeader.biSizeImage,1,filePtr);    // Read the Bitmap-data
    fclose(filePtr);
    return bitmapImage;
}
