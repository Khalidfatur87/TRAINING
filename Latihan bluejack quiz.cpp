#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Game
{
	int stock;
	char title[25];
	char genre[10];
	
	struct Game* left;
	struct Game* right;
	int height;
};

int max(int a, int b)
{
	return (a > b) ? a : b;
}

int height(struct Game* node)
{
	if (node == NULL)
	{
		return 0;
	}
	else
	{
		return 1 + max(height(node->left), height(node->right));
	}
}

int get_balance(struct Game* node)
{
	if (node == NULL)
	{
		return 0;
	}
	else
	{
		return height(node->left) - height(node->right);
	}
}

struct Game* new_node(char* title, char* genre, int stock)
{
	struct Game* game = (struct Game*)malloc(sizeof(struct Game));
	strcpy(game->title, title);
	strcpy(game->genre, genre);
	game->stock = stock;
	
	game->left = NULL;
	game->right = NULL;
	game->height = 1;
	
	return game;
}

struct Game* RightRotate(struct Game* node)
{
	struct Game* x = node->left;
	struct Game* sub_x = x->right;
	
	x->right = node;
	node->left = sub_x;
	
	node->height = max(height(node->left), height(node->right)) + 1;
	x->height = max(height(node->left), height(node->right));
	
	return x;
}

struct Game* LeftRotate(struct Game* node)
{
	struct Game* x = node->right;
	struct Game* sub_x = x->left;
	
	x->left = node;
	node->right = sub_x;
	
	node->height = max(height(node->left), height(node->right)) + 1;
	x->height = max(height(node->left), height(node->right));
	
	return x;
}

struct Game* Insertion(struct Game* node, char* title, char* genre, int stock)
{
	if (node == NULL)
	{
		return new_node(title, genre, stock);
	}
	
	if (stock < node->stock)
	{
		node->left = Insertion(node->left, title, genre, stock);
	}
	else if (stock > node->stock)
	{
		node->right = Insertion(node->right, title, genre, stock);
	}
	else
	{
		return node;
	}
	
	node->height = max(height(node->left), height(node->right)) + 1;
	
	int balance = get_balance(node);
	
	//LL Rotation
	if (balance > 1 && stock < node->left->stock)
	{
		return RightRotate(node);
	}
	
	//LR Rotation
	else if (balance > 1 && stock > node->left->stock)
	{
		node->left = LeftRotate(node->left);
		return RightRotate(node);
	}
	
	//RR Rotation
	else if (balance < -1 && stock > node->right->stock)
	{
		return LeftRotate(node);
	}
	
	//RL Rotation
	else if (balance < -1 && stock < node->right->stock)
	{
		node->right = RightRotate(node->right);
		return LeftRotate(node);
	}
	
	return node;
}

struct Game* find_mid(struct Game* node)
{
	struct Game* current = node;
	
	while (current && current->left != NULL)
	{
		current = current->left;
	}
	
	return current;
}

struct Game* search(struct Game* node, char* title)
{
	if (node == NULL || strcmp(node->title, title) == 0)
	{
		return node;
	}
	
	if (strcmp(title, node->title) < 0)
	{
		return search(node->left, title);
	}
	else
	{
		return search(node->right, title);
	}
}

struct Game* Deletion(struct Game* node, char* title)
{
	if (node == NULL)
	{
		return node;
	}
	
	if (strcmp(title, node->title) < 0)
	{
		node->left = Deletion(node->left, title);
	}
	else if (strcmp(title, node->title) > 0)
	{
		node->right = Deletion(node->right, title);
	}
	else
	{
		if (node->left == NULL)
		{
			struct Game* temp = node->right;
			free(node);
			return temp;  
		}
		else if (node->right == NULL)
		{
			struct Game* temp = node->left;
			free(node);
			return temp;
		}
		
		struct Game* temp = find_mid(node->right); 
		strcpy(node->title, temp->title);
		strcpy(node->genre, temp->genre);
		node->stock = temp->stock;
		node->right = Deletion(node->right, temp->title);
	}
	
	return node;
}


void Inorder(struct Game* node)
{
	if (node != NULL)
	{
		Inorder(node->left);
		printf("|%-25s | %-10s | %-3d|\n", node->title, node->genre, node->stock);
		for(int i=0; i<=45; i++)
		{
			printf("-");
		}
		printf("\n");
		Inorder(node->right);
	}
}

int main()
{
	struct Game* root = NULL;
	struct Game* searching;
	
	
	int menu_option;
	
	char UpdateType[10];
	int UpdateValue;
	
	char GameTitle[25];
	char GameGenre[10];
	int GameStock;
	
	do
	{
		printf("Bluejack GShop\n");
		printf("==============\n");
		printf("1. Insert Game\n2. View Game\n3. Update Stock\n4. Exit\n>> ");
		scanf("%d", &menu_option);
		getchar();
		
		switch(menu_option)
		{
			case 1:
				
				do
				{
					printf("Input game title[5-25][unique]: ");
					scanf(" %[^\n]", GameTitle);
					getchar();
					
					searching = search(root, GameTitle);
				} while(strlen(GameTitle) <= 5 || strlen(GameTitle) >= 25 || searching != NULL);
				
				do
				{
					printf("Input game type[Action|RPG|Adventure|Card Game]: ");
					scanf(" %[^\n]", GameGenre);
					getchar();
				} while(strcmp(GameGenre, "Action") != 0 && strcmp(GameGenre, "RPG") != 0 && strcmp(GameGenre, "Adventure") != 0 && strcmp(GameGenre, "Card Game") != 0);			
				
				do
				{
					printf("Input game stock[>= 1]: ");
					scanf("%d", &GameStock);
					getchar();
				} while(GameStock < 1);
				
				root = Insertion(root, GameTitle, GameGenre, GameStock);
				
				printf("Insert sucess!\n\n");
				printf("Press enter to continue...");
				
				getchar();
				break;
			
			case 2:
				
				if (root == NULL)
				{
					printf("Warehouse is empty!\n");
					printf("\nPress enter to continue...");
				}
				else
				{
					for(int i=0; i<=45; i++)
					{
						printf("-");
					}
					printf("\n");
					Inorder(root);
					
					printf("\nPress enter to continue...");
				}

				getchar();
				break;
			
			case 3:
				
				printf("Input game title: ");
				scanf(" %[^\n]", GameTitle);
				getchar();
				
				searching = search(root, GameTitle);
				
				if (searching == NULL)
				{
					printf("Data not found!\n\n");
					printf("Press enter to continue...");	
				}
				else
				{
					printf("Current stock: %d\n\n", searching->stock);
					
					do
					{
						printf("Input update type[add|remove][case insensitive]: ");
						scanf("%s", UpdateType);
					} while(strcmpi(UpdateType, "add") != 0 && strcmpi(UpdateType, "remove") != 0);
					
					if (strcmpi(UpdateType, "add") == 0)
					{
						do
						{
							printf("Input stock to add[>= 1]: ");
							scanf("%d", &UpdateValue);
						} while(UpdateValue < 1);
						
						int NewValue = searching->stock + UpdateValue;
						
						searching->stock = NewValue;
						
						printf("Data updated succesfully!\n");
						printf("\nPress enter to continue...");
						getchar();
					}
					else if (strcmpi(UpdateType, "remove") == 0)
					{
						do
						{
							printf("Input stock to remove[1-%d]: ", searching->stock);
							scanf("%d", &UpdateValue);
						} while(UpdateValue < 1 || UpdateValue > searching->stock);
						
						int NewValue = searching->stock - UpdateValue;
						searching->stock = NewValue;
						
						if (NewValue == 0)
						{
							printf("%s is removed from the warehouse!\n", searching->title);

							root = Deletion(root, GameTitle);
						}	
						
						printf("\nPress enter to continue...");
						getchar();
					}
				}
				
				getchar();
				break;
			
			default:
				printf("Task not available! Press enter to continue.....\n");
				getchar();
				break;
				
		}
		
		system("cls");
	} while (menu_option != 4);
	
	return 0;
}
