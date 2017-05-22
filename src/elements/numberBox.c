#include "../../includes/game.h"

/*Create one button with the numberbox texture, add a text element on top, and link interactions between the two*/
NumberBox * createNumberBox(int x, int y, int width, int height, int value, int min, int max)
{
	NumberBox * nBox = allocate(sizeof(NumberBox));
	char * caption;

	nBox->value = value;
	nBox->min = min;
	nBox->max = max;

	/*Create text*/
	caption = itoa(value);
	nBox->display = createText(caption, x, y - 14, gameObj.defaultFont);
	free(caption);

	/*Create the button*/
	nBox->btn = createButton(x, y, width, height, 0);
	nBox->btn->idleTexture = getTexture("numberBox_idle.png");
	nBox->btn->selectedTexture = getTexture("numberBox_selected.png");
	nBox->btn->isNumberBox = true;
	nBox->btn->callbackArgument = nBox;
	nBox->btn->text = nBox->display;

	return nBox;
}


/*Free the number box, the button and the text are not free-ed*/
void freeNumberBox(NumberBox * nBox)
{
	free(nBox);
	nBox = NULL;
}


/*Increment by one the number box, and update it's text*/
void incrementNumberBox(NumberBox * nBox)
{
	char * caption;

    if(nBox->value >= nBox->max)
		return;

    nBox->value++;

	caption = itoa(((NumberBox*)nBox)->value);
	strcpy(nBox->display->text, caption);
	free(caption);
}


/*Decrement by one the number box, and update it's text*/
void decrementNumberBox(NumberBox * nBox)
{
	char * caption;

    if(((NumberBox*)nBox)->value <= ((NumberBox*)nBox)->min)
		return;

    ((NumberBox*)nBox)->value--;

	caption = itoa(((NumberBox*)nBox)->value);
	strcpy(((NumberBox*)nBox)->display->text, caption);
	free(caption);
}
