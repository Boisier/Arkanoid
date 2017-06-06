#include "../../includes/game.h"

/** Parse the given folder and return a FolderContent object with a list of containing files **/
FolderContent * parseFolder(char * folderPath)
{
    struct dirent * element = NULL;
	FolderContent * content = allocate(sizeof(FolderContent));

	/*Init FolderContent*/
	content->nbrElements = 0;
	content->elements = NULL;

	/*Open directory*/
	content->dir = opendir(folderPath);

	/*Stop here if directory not found*/
	if(content->dir == NULL)
	{
		printf("Error %d oppening %s : %s\n", errno, folderPath, strerror(errno));
		throwCriticalError();
		return content;
	}

	/*Loop on each element*/
	while((element = readdir(content->dir)) != NULL)
	{
		/*If dot folder, go to the next loop*/
		if(element->d_name[0] == '.')
			continue;
		
		content->nbrElements++;
		
		/*Add space to store the new element*/
		content->elements = reAllocate(content->elements, sizeof(struct dirent *) * content->nbrElements);

		/*Store the new element*/
		content->elements[content->nbrElements - 1] = element;
	}

	/*return FolderContent*/
	return content;
}


/** Close and free a FolderContent **/
void closeFolder(FolderContent * folder)
{
	(void)closedir(folder->dir);

	free(folder->elements);
	free(folder);
	folder = NULL;
}
