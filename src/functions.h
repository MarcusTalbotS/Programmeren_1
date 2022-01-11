#ifndef EINDOPDRACHT_FUNCTIONS_H
#define EINDOPDRACHT_FUNCTIONS_H

// Final character-set used in the assignment, containing 70 character.
const char *charset = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

int calculateAverageGreyValue(int rows, int columns, int imageArray[rows][columns]);

void loadImageFromFile(const char *filePath, int size, int imageArray[size][size]);

char **greyValuesToChars(int rows, int columns, int imageArray[rows][columns]);

int writeImageToFile(const char *filePath, int rows, char **imageArray);

void printASCIIImage(int size, char **imageArray);

void printReverseASCIIImage(int size, char **imageArray);

unsigned char *readGreyscaleBitmap(const char *filePath);

#endif //EINDOPDRACHT_FUNCTIONS_H
