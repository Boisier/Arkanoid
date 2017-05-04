#pragma once

/******************************/
/** SCREEN DISPLAY FUNCTIONS **/
/******************************/

/** Print the toPrint array on the screen **/
void printScreen();

/** Add an element to the toPrint array **/
PrintElement * addToPrint(void * element, enum elType type);

/** Delete an element **/
void deleteElement(PrintElement el);

/** Empty the toPrint array **/
void cleanToPrint();

/** remove deleted element and close the gaps **/
void consolidateToPrint();
