
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *banner = "--------------------------------------\n";

struct Product {
	char* name;
	double price;
};

struct ProductStock {
	struct Product product;
	int quantity;
};

struct Shop {
	double cash;
	struct ProductStock stock[10];
	int index;
};

struct Customer {
	char* name;
	double budget;
	struct ProductStock shoppingList[10];
	int index;
};

void printProduct(struct Product p)
{
	printf("PRODUCT NAME:\t%s \nPRODUCT PRICE:\t%.2f\n", p.name, p.price);
}

void printCustomer(struct Customer c)
{
	printf("\n\n%s\t\tCUSTOMER\n%s",banner,banner);
	printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n", c.name, c.budget);
	printf(banner);
	double sum = 0;
	for(int i = 0; i < c.index; i++)
	{
		printProduct(c.shoppingList[i].product);
		printf("QUANTITY:\t%d\n",c.shoppingList[i].quantity);
		double prod = c.shoppingList[i].quantity * c.shoppingList[i].product.price; 
		printf("TOTAL:\t\t%.2f\n\n", prod);
		sum+=prod;
	}
	printf("ESTIMATED VALUES ONLY \n");
	printf("TOTAL COST:\t%.2f\n",sum);
	printf("BUDGET BALANCE:\t%.2f\n",c.budget-sum);
}

void printShop(struct Shop s)
{
	
	printf("\n\n%s\t\tSHOP\n%s",banner,banner);
	printf("CASH:\t\t€ %.2f", s.cash);
	printf("\n%s",banner);
	for (int i = 0; i < s.index; i++)
	{
		printProduct(s.stock[i].product);
		printf("STOCK LEVEL:\t%d ", s.stock[i].quantity);
		printf("\n\n");
	}
}

const char * stripNewline(char *textStr){
	// strip off \n character
	if (textStr[strlen(textStr)-1] == '\n'){textStr[strlen(textStr)-1] = '\0';}
	// strip off \r character
	if (textStr[strlen(textStr)-1] == '\r'){textStr[strlen(textStr)-1] = '\0';	}
	// strip leading white space
	while(isspace((unsigned char)*textStr)){textStr++;}
	return textStr;
} 

struct Customer createAndLoadShoppingList(char *csvfile){
	struct Customer customer = { 0 };
	FILE * fp;
	char * line = NULL;
	size_t length = 0;
	ssize_t read;
	char *tokens[3];
	
	fp = fopen(csvfile, "r");
	if (fp == NULL)
			exit(EXIT_FAILURE);

	// read the csv file line by line in the while loop
	while ((read = getline(&line, &length, fp)) != -1) {
		int idx=0; // reset the tokens array index
		char *t = strtok(line, ","); // read the first value untill a comma is encountered
		// repeat reading untill a null charecter is encountered
		while ( t != NULL ){
			char *token = malloc(sizeof(char) * 25); //create a local token string variable
			strcpy(token,stripNewline(t)); // copy the value t to token after cleanup
			tokens[idx++]=token; // add all the tokens to the token array for further processing
			t = strtok(NULL, ",");//read the next token untill null
		}
		// if the customer name was read in, run this code
		if (strstr(tokens[0],"Name") != NULL|strstr(tokens[0],"name") != NULL)
		{
			// add the customer name to the customer structure variable name
			customer.name = tokens[1];
		}
		// otherwise if the budget is returned execute this code
		else if (strstr(tokens[0],"Budget") != NULL|strstr(tokens[0],"budget") != NULL)
		{
			double budget = atof(tokens[1]);// convert the budget value to a float
			// and assign it to the customer structure 
			customer.budget = budget;
		}
		//otherwise add the shopping list quantity and cost to the customer in a structured way
		//and increment the index
		else
		{
			int n = atoi(tokens[1]); // convert the number of items to an interger value
			//double cost = atof(tokens[1]);// convert the cost to a floating point value
			double cost = 0;
			//create a product structure with a name and cost
			struct Product product = { tokens[0], cost };
			//and add the product structure and number of the items to the shopping list
			struct ProductStock shopList = { product, n };
			//and update the shoppinglist with the content while also incrementing the index
			customer.shoppingList[customer.index++] = shopList;
		}
	}	
	//and when the loops conclude return the customer structre to the calling function
	return customer;
}


struct Shop createAndStockShop()
{
	struct Shop shop = { 0 };
	FILE * fp;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen("stock.csv", "r");
	if (fp == NULL) 
			exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
			// printf("Retrieved line of length %zu:\n", read);
			// printf("%s IS A LINE", line);
	if (strstr(line,"cash") != NULL|strstr(line,"Cash") != NULL){
		char *n = strtok(line, ",");
		char *c = strtok(NULL, ",");
		double cash = atof(c);
		shop.cash = cash;
	}
	else {
		char *n = strtok(line, ",");
		char *p = strtok(NULL, ",");
		char *q = strtok(NULL, ",");
		int quantity = atoi(q);
		double price = atof(p);
		char *name = malloc(sizeof(char) * 50);
		strcpy(name, n);
		//prepare product and stockItems structures
		struct Product product = { name, price };
		struct ProductStock stockItem = { product, quantity };
		//two things happen here, shop.tock at index[0] is updated but also incremented and 
		// is updated with value stockItem.
		shop.stock[shop.index++] = stockItem;
		// printf("NAME OF PRODUCT %s PRICE %.2f QUANTITY %d\n", name, price, quantity);
	}
	
	}
	
	return shop;
}

struct Shop processOrder(struct Shop s, struct Customer c){

	double total=0; //create  variable for adding up the amount due for the products in stock
	printf("\n\n%s\tPROCESSING ORDER\n%s",banner,banner);
	// create an outer loop and loop through the shopping list
	for (int i=0;i<c.index;i++){
		short isInShop = 0;
		char *list = malloc(sizeof(char) * 25);
		strcpy(list,  c.shoppingList[i].product.name);
		// and create an inner loop and loop through the shop
		for (int j = 0; j < s.index; j++){
			char *shop = malloc(sizeof(char) * 25);
			strcpy(shop,  s.stock[j].product.name);
			//find the item in the shopping list in the shop
			if (strstr(list, shop) != NULL){//if shopping list item is in shope
				isInShop=1;
				printf("IN STOCK:");
				// now check if the order quantity can be filled from stock
				int qOrder = c.shoppingList[i].quantity;
				int qShop = s.stock[j].quantity;
				double price = s.stock[j].product.price;
				if (qOrder<qShop){ //the order can be fully filled
					printf(" %s x %i @ %.2f\n",shop, qOrder, price);
					// if the order can be filled, remove the item from the shop stock
					s.stock[j].quantity-=qOrder;//reduce the shop stock
					s.cash+=(s.stock[j].product.price*qOrder);//increase the shop cash
					total+=(qOrder*price);
				}
				else { //the order cannot be fully filled, add what is available
					printf(" %s x %i @ %.2f , %i short\n",shop, qShop, price, qOrder-qShop);
					s.stock[j].quantity-=qShop;
					s.cash+=(s.stock[j].product.price*qShop);
					total+=(qShop*price);
				}
			}
			//reached the end of the list and item was not found
			if (j == s.index-1 & !isInShop) { //shopping list item is not is shop
				printf("NO STOCK: %s\n", list);
			}
		}
	}
	printf("%s   TOTAL:\t%4.2f\n",banner,total);//print out the total
	printf(" BALANCE:\t%4.2f\n",c.budget-total);//print oiut the balance left from the budget submitted
	return s;
}

void saveShopState(struct Shop s,char * filename){
	//a function to save the shop status back after completing the transactions submitted
	//s.cash;
	//s.index;
	//s.stock[0].quantity;
	//s.stock[0].product.name;
	//s.stock[0].product.price;
	
	FILE * fp;
	fp=fopen(filename,"w+");
	fprintf(fp,"Cash, %.2f\n",s.cash);
	for (int i=0;i<s.index;i++){
		fprintf(fp, "%s, %.2f, %i\n",s.stock[i].product.name,s.stock[i].product.price,s.stock[i].quantity);
	}
	fclose(fp);
	return;
}

char displayShopMenu()
{
	static int ShopLoaded = 0, ListLoaded = 0 ;
	system("clear");
	printf("%s\tINTERACTIVE SHOP MENU\n%s",banner,banner);
	printf("STOCKED: %i LIST: % i\n",ShopLoaded, ListLoaded);
	printf("%s",banner);
	printf("\tl - Load Shop Stock from csv\n");
	printf("\tp - Print Shop Stock\n");
	printf("\ts - Load the shopping list from csv\n");
	printf("\tr - Print the shopping list\n");
	printf("\tm - Manually Enter Shopping list\n");
	printf("\th - Process shopping list\n");
	printf("\tx - Exit Application\n");
	printf("\n  Select option please: ");
	char c = getchar();
	printf("\n");
	if (c=='l'){ShopLoaded=1;}
	if (c=='s'){ListLoaded=1;}
	return c;
} 

void enterManualList()
{
	char name[20];
	float budget;
	int loop = 1;
	char items[20][20];
	char tempStr[20];
	int quantities[20];
	int index=0;

	printf("You are now at the manual entry shop.\n");
	printf("Please enter your name: ");	
	getchar();
	scanf("%20[^\n]",&name);
	printf("Please enter your budget amount: ");
	scanf("%s",&tempStr);
	budget=atof(tempStr);
	printf("NAME: '%s' BUDGET: '%.2f'\n", name, budget);
	//fflush(stdin);
	while (loop==1)
	{
		char c;
		printf("Please enter the item name to add: ");
		//gets(tempStr);
		getchar();
		scanf("%20[a-zA-z ]",&items[index]);
		//printf("%s\n",items[index]);
		printf("Please enter the item quantity to add: ");
		//gets(tempStr);
		scanf("%s",&tempStr);
		//printf("%s\n",tempStr);
		quantities[index] =  atoi(tempStr);
		printf("Do you want to add another item? (y/n)");
		getchar();
		c = getchar();
		if (c=='n')
		{
			loop=0;
		}
		else if (c=='y')
		{
			index++;
		}
	}
	//now save this to the csv file
	FILE *fp = fopen("order.csv", "w");
	fprintf(fp,"Name,  %s\nBudget, %f\n",name,budget);
	for (size_t i = 0; i <= index; i++)
	{
		fprintf(fp,"%s, %i\n", items[i], quantities[i]);
	}
	fclose(fp);
	printf("List saved to file order.csv\n");
	printf(" <ENTER> to continue ");
	getchar();
	getchar();
}

int main(void) 
{
	struct Shop shop;
	struct Customer customer;
	int loop = 1;//set to one to use menu
	while (loop==1){
		char c = displayShopMenu();
		//printf("returned: %c", c);
		if (c=='x') //exit
		{
			loop=0; //loop will break orr pass return
			printf("  Exiting interactive mode, saving shop state...\n");
			saveShopState(shop,"stock.csv"); //on exit save the shop state
		}
		else if (c=='l') //load shop 
		{
			printf("  Loading stock now ....\n");
			shop = createAndStockShop();
			getchar();
		}	
		else if (c=='p') //print shop
		{
			printShop(shop);
			printf(" <ENTER> to continue ");
			getchar();
			getchar();
		}
		else if (c=='s') //load shopping list
		{
			printf("  Loading shopping list ....\n");
			customer = createAndLoadShoppingList("order.csv");
			//printf(" <ENTER> to continue ");
			//getchar();
			getchar();
		}
		else if (c=='r') //print shopping list
		{
			printCustomer(customer);
			printf(" <ENTER> to continue ");
			getchar();
			getchar();
		}
		else if (c=='m')
		{
			enterManualList();
		}
		else if (c=='h') //process the shopping list against the shop stock
		{
			shop = processOrder(shop,customer);
			printf(" <ENTER> to continue ");
			getchar();
			getchar();
		}
		
		else
		{
			printf("\n  Invalid option!\n  Try again please.\n\n");
			printf(" <ENTER> to continue ");
			getchar();
			getchar();
		}
		
	}

	//struct Shop shop = createAndStockShop();
	//printShop(shop);
	//struct Customer customer = createAndLoadShoppingList("order.csv");
	// customer = createAndLoadShoppingList("order.csv");
	//printCustomer(customer);
	//shop = processOrder(shop,customer);
	//printShop(shop);
	//saveShopState(shop,"stock.csv");
  return 0;
}
