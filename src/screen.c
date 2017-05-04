#include "../includes/game.h"

/** Print the toPrint array on the screen **/
void printScreen()
{
    int nbrToPrint = gameObj.nbrToPrint;
    int i;
    bool deletion = false;
    PrintElement el;

    /*No need to go further if there's nothing to print*/
    if(gameObj.printContent == EMPTY || gameObj.nbrToPrint == 0)
        return;

    /** Clean screen ?**/                            
    
    /*Iterate over all the element of the toPrint array*/
    for(i = 0; i < nbrToPrint; i++)             
    {
        el = gameObj.toPrint[i];

        if(!el.display) /*Deactivated element ?*/
            continue;

        switch(el.type)          
        {
            case PLATEFORME: printPlateforme(el.element.plateforme); break;
            case PICTURE: printPicture(el.element.pict); break;
            case BRICK: printBrick(el.element.brick); break;
            case BALL: printBall(el.element.ball); break;
            case BONUS: printBonus(el.element.bonus); break;
            case BUTTON: printButton(el.element.btn); break;
            case NUMBERBOX: break;
            case TEXT: printText(el.element.txt); break;
        }
    }

    if(deletion)
        consolidateToPrint();
}

/** Add an element to the toPrint array **/
PrintElement * addToPrint(void * element, enum elType type)
{
    int newSize;
    PrintElement newEl;
    
    /*We make sure no NULL pointer get's in the loop*/
    criticalIfNull(element);                    
    
    newEl.type = type;                             
    
    switch(type)          
    {
        case PLATEFORME: newEl.element.plateforme = element; break;
        case BRICK: newEl.element.brick = element; break;
        case BALL: newEl.element.ball = element; break;
        case BONUS: newEl.element.bonus = element; break;
        case BUTTON: newEl.element.btn = element; break;
        case PICTURE: newEl.element.pict = element; break;
        case NUMBERBOX: 
            addToPrint(((NumberBox*)element)->btn, BUTTON);
            addToPrint(((NumberBox*)element)->display, TEXT);
            return NULL;
        break;
        case TEXT: newEl.element.txt = element; break;
    }

    newEl.display = true;
    
    /*Add room in the toPrint array to store this new element*/
    newSize = (gameObj.nbrToPrint + 1) * sizeof(PrintElement);

    if(gameObj.nbrToPrint == 0)
        gameObj.toPrint = allocate(newSize);
    else
        gameObj.toPrint = reAllocate(gameObj.toPrint, newSize); 

    /*Insert newly created printElement in the toPrint array*/
    gameObj.toPrint[gameObj.nbrToPrint] = newEl;                 
    gameObj.nbrToPrint++;
    
    /*Return the newly created element so it can be edited*/
    return &gameObj.toPrint[gameObj.nbrToPrint-1];       
}

/** Empty the toPrint array **/
void cleanToPrint()
{
    int i;

    if(gameObj.nbrToPrint != 0)
    {
        for(i = 0; i < gameObj.nbrToPrint; i++)
        {
            deleteElement(gameObj.toPrint[i]);
        }

        free(gameObj.toPrint); 
        
        gameObj.toPrint = NULL;
        gameObj.nbrToPrint = 0;                                     
    }
}

/** Delete an element **/
void deleteElement(PrintElement el)
{
    /*Free the element with the appropriate function*/
    switch(el.type)          
    {
        case PLATEFORME: freePlateforme(el.element.plateforme); break;
        case BRICK: freeBrick(el.element.brick); break;
        case BALL: freeBall(el.element.ball); break;
        case BONUS: freeBonus(el.element.bonus); break;
        case BUTTON: freeButton(el.element.btn); break;
        case PICTURE: freePicture(el.element.pict); break;
        case NUMBERBOX: freeNumberBox(el.element.nBox); break;
        case TEXT: freeText(el.element.txt); break;
    }
}
