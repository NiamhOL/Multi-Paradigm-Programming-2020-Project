// C library header files
#include <stdio.h>     // Needed to perform standard IO operations e.g. scanf() and printf()
#include <string.h>    // Needed for string manipulation operations e.g. strlen and strcpy
#include <stdlib.h>    // Needed for standad Utility functions e.g. dynamic memory allocation using functions like malloc() and calloc().
#include <ctype.h>     // Needed for character type functions e.g.  isaplha() and isdigit()


// Product entity - defines the products the shop sells

struct Product {
	char* name;            // * is a pointer that allows it to expand into memory
	double price;         // Double allows for a decimal number
};


//Stock entity - This contains a struct inside a struct (nested struct)

struct ProductStock {
	struct Product product;
	int quantity;
};


// Customer entity

struct Customer {
	char* name;      
	double budget;          
	struct ProductStock shoppingList[30];       
	int index;      // keeps track of how many elements have been stored in customers shoppingList
};

// Shop entity- again we have a struct inside a struct because we not only want to know the product but how many of such product there is

struct Shop {
	double cash;					// opening cash in shop
	struct ProductStock stock[20];  // array of stock items - 20 items in the array
	int index;                     // keeps track of how many items have been added and read in from the files
};


// Method: Print out product information - name and price.
void printProduct(struct Product p) {

	printf("PRODUCT NAME: %s \nPRODUCT PRICE: €%.2f\n", p.name, p.price);
};

struct Product getProduct(struct Shop s, char* pname){
   // check the shop for a product and return it
   struct Product p;
   for (int i = 0; i < s.index; i++){
      if(strcmp(s.stock[i].product.name,pname)==0){
        p = s.stock[i].product;
      }
   }
   return p;
};


// Prints out the customer order that they wish to order
void printCustomer(struct Customer c) {
	
	// Print the customer budget which is stored in the csv file 
	printf("\n\n\nCUSTOMER NAME   : %s \nCUSTOMER BUDGET : €%.2f\n", c.name, c.budget);	

	printf("\n\n******************************************************\n");
	printf("\n***                CUSTOMER ORDER                  ***\n");
	printf("\n******************************************************\n");
	printf("\n| PRODUCT             |  QTY | UNIT PRICE |Total Price|\n");
	printf("\n******************************************************\n");
	
    double charge = 0.0;
	// For loop that passing each product in the customer shopping list - prints name, cost and quantity
	for (int i = 0; i < c.index; i++) {
		
		// printProduct(c.shoppingList[i].product.name);
		// printf("QUANTITY: %d\n", c.shoppingList[i].quantity);
        // Calculate the totalPrice = (unit price x quantity)
        
		double totalPrice = c.shoppingList[i].quantity * c.shoppingList[i].product.price; 		
		
		printf("| %-20.20s| %.5d| €%10.2f| €%8.2f|\n", 
                c.shoppingList[i].product.name, 
			    c.shoppingList[i].quantity,
			    c.shoppingList[i].product.price, totalPrice);
 
		printf("\n******************************************************\n");

		// Adds the totalPrice (unit cost x quantity) to the charge
		charge += totalPrice; 	

	}
	// THis will be calculated before we determine if the items are stocked or not 
	printf("\n\n%s's PROVISIONAL TOTAL COST OF ITEMS :     € %.2f\n", c.name, charge);
	printf("\n\n%s's PROVISIONAL UPDATED BUDGET      :     € %.2f\n", c.name, (c.budget - charge));
	printf("\n******************************************************\n");

}


// Read in the contents of the CSV file
struct Shop createAndStockShop() {

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    // read the stock csv file
    fp = fopen("stock.csv", "r");

    // Error Handling: if file doesn't exist exit program
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // Get first line from shop stock csv
    getline(&line, &len, fp);                  // Read cash from csv file 
    double cash = atof(line);                 // atof converts to float point number 
    struct Shop shop = {cash};               // Shop struct which sets the shop cash initially to the first line in the file    

   // Loop through remainder of stock file - read line by line until it reaches the end (!=-1)
    while ((read = getline(&line, &len, fp)) != -1) {
			
        // getting name, price and quantity - "," is specifying the break
        char *n = strtok(line, ",");  // strtok we want to pass the line which we have read in
        char *p = strtok(NULL, ",");  // NULL means we are not passing any information
        char *q = strtok(NULL, ",");  
        
        // Converts datatypes
        double price = atof(p);                       // atof converts string to floating point number
        int quantity = atoi(q);                       // atoi converts string to integer
        char *name = malloc(sizeof(char) * 50);      // allows memory to be dynamically allocated for this pointer name. malloc means to manually allocate a piece of memory on the computer of a certain size for us to store out information while the program is running.  allocation where we set the character size limit to 50
        strcpy(name, n);                            // strcpy function means it takes from the source n and makes a copy of the data and puts it into the new memory location represented in the line above 
        
        //Reads file in line by line then add product variable and then add both to the shop
        struct Product product = {name, price};
        struct ProductStock stockItem = {product, quantity};
            
       // shop.stock at index[0] is updated but also incremented and is updated with value stockItem.
        shop.stock[shop.index++] = stockItem;	
	}
    return shop;
};

// Customer order - read in customer shopping list from csv fie 
struct Customer customerOrder(struct Shop s, char* csvfile) {
   
	FILE * fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

   // read the order csv file. 
	fp = fopen(csvfile, "r"); 

   // Error Handling: if file doesn't exist exit program
    if (fp == NULL)
        exit(EXIT_FAILURE);

   // read the first line from the file -get the customer name and budget
	getline(&line, &len, fp);
	char *cn = strtok(line, ",");
	char *cb = strtok(NULL, ",");
	
	//Customer name
	char *cname = malloc(sizeof(char) * 100);	  
	strcpy(cname, cn);                              // new memory location to store n as name
	double cbudget = atof(cb);                      // Converts the budget to a double 

	struct Customer customer = {cname, cbudget};    // Creates a customer with budget 
    
    // while loop will pass through the rest of the file and create customer shopping list 
    while ((read = getline(&line, &len, fp)) != -1) {
		
		char *pn = strtok(line, ",");                   
		char *qt = strtok(NULL, ",");
		
		// Product to buy... price & product name
		int orderQuantity  = atoi(qt);                 
		char *name = malloc(sizeof(char) * 50);	//new memory location to store n as name
		strcpy(name, pn);
	
		//struct Product product = {name};
		struct Product product = {name, getProduct(s, name).price};
		struct ProductStock listItem = {product, orderQuantity};
		
		// Populate the Array to contain list items for this customer
		customer.shoppingList[customer.index++] = listItem;
		
    }
	return customer;
};


void printShop(struct Shop s) {
	// Print the current cash balance for the shop which is stored in csv file 
	printf("\n\n\n SHOP BALANCE : €%.2f \n", s.cash);

	printf("\n\n*******************************************\n");
	printf("\n***            SHOP INVENTORY           ***\n");
	printf("\n*******************************************\n");
	printf("\n| PRODUCT             |   PRICE  |   QTY  |\n");
	printf("\n*******************************************\n");
	
	// For loop that passing each product in the shop stock - prints name, price and quantity
	for (int i = 0; i < s.index; i++) {

		// printProduct(s.stock[i].product);
		// printf("QUANTITY: %d\n", s.stock[i].quantity);
		
		// Print the current amount of stock available and price for each product in the shop
		printf("| %-20.20s| €%8.2f|%8.4d|", s.stock[i].product.name, s.stock[i].product.price, s.stock[i].quantity);
		printf("\n*******************************************\n");
	
	}
}

char* findProduct(struct Shop* s, char *n){
   //take in a shop and a string
   // loop through the shop stock
	for(int j = 0; j < s->index; j++){
      // check if the item entered is in stock
		if(strcmp(s->stock[j].product.name,n)==0){
         // if so return the product name
			return s->stock[j].product.name;   
         }         
   }
   //if not return "Not in Stock"
	return "NULL";
}



void reviewOrder(struct Shop* s, struct Customer* c) {


    // Create the following variables:

    double orderCost = 0;             // Variable to  calculate the amount due for products stocked in shop
	double shortAmount = 0;           // Variable to calculate amount the customer is short when paying for items
	double shortStock = 0;            // Variable to calculate the shortage in stock in the shop vs what the customer ordered

	
	printf("\n\n******************************************************\n");
	printf("\n***            REVIEW AND PROCESS ORDER            ***\n");
	printf("\n******************************************************\n");
    
    // Outer for loop which loops through customer shopping list 
    for(int i = 0; i < c->index; i++) {

    // Calculate the total cost of the order by multiplying the item price by quantity customer wants
    orderCost += (c->shoppingList[i].quantity * c->shoppingList[i].product.price);

    }

    // Dilemma 2: Product is not stocked in the shop. 

	for(int i = 0; i < c->index; i++){
	
		if (strcmp(findProduct(s, c->shoppingList[i].product.name),"NULL")==0){
			
			printf("\nSORRY %s %s IS NOT STOCKED BY OUR SHOP AT THIS TIME!\n", c->name, c->shoppingList[i].product.name);
			return;
		}

	}


	// Dilemma 1: Customer does not have enough money - customer budget is less than total cost of the products. Transaction will be cancelled as a result
	
	if(orderCost > c->budget){
		
        // Find the amount the customer is short by deducting the customer budget from the total order cost
		shortAmount = (orderCost - c->budget);  
		printf("\n\n*****************************************************************************\n");
		printf("\nSORRY YOUR BUDGET DOES NOT COVER THE COST OF THE PRODUCTS YOU WISH TO PURCHASE!");
		printf("\nYOU ARE SHORT BY €%.2f \n", shortAmount);
		printf("\n*******************************************************************************\n");
      	return;
    }

// Dilemma 3: Shop stock is less than the quantity the customer wishes to order

    // Outer for loop which loops through customer shopping list 
    for(int i = 0; i < c->index; i++) {

        // Inner for loop which loops through the shop stock
        for(int j = 0; j < s->index; j++){
        
            // Find if the item in the shopping list is in the shop  - this checks if the strings are the same 
            // https://c-for-dummies.com/blog/?p=2641
            if(strcmp(c->shoppingList[i].product.name, s->stock[j].product.name)==0) {
            
                // if the shop stock quantity is less than the customer order quantity
                if(c->shoppingList[i].quantity > s->stock[j].quantity) {
					shortStock = (c->shoppingList[i].quantity -  s->stock[j].quantity);

                    printf("\nSORRY %s OUR STORE ONLY HAS %d %s!\n", c->name, s->stock[j].quantity, c->shoppingList[i].product.name);
                    printf("THERE ARE %lf %s MISSING FROM YOUR ORDER\n", shortStock, c->shoppingList[i].product.name );
					printf("%s WILL NOT BE ADDED TO YOUR ORDER\n", c->shoppingList[i].product.name);
					printf("PLEASE ADJUST ORDER ON FILE FOR %s AND REORDER\n", c->shoppingList[i].product.name);
					printf("ALL OTHER PRODUCTS LISTED ABOVE WILL BE ADDED AND PROCESSED\n");
					return;
                } 
                
                // Update the shop stock quantity to reflect the customer purchasing products
                s->stock[j].quantity = s->stock[j].quantity - c->shoppingList[i].quantity;

                // Update the shop balance to reflect the customer purchasing products
                s->cash = s->cash + (c->shoppingList[i].product.price * c->shoppingList[i].quantity);
                
                // Update the customer budget to reflect the purchase of products from the shop
                c->budget = c->budget - (c->shoppingList[i].product.price * c->shoppingList[i].quantity); 	

				printf("\n%s HAS BEEN SUCCESSFULLY ORDERED\n", c->shoppingList[i].product.name);
				printf("TOTAL COST OF %s                  :         € %.2f\n", c->shoppingList[i].product.name,  (c->shoppingList[i].product.price * c->shoppingList[i].quantity));
				printf("UPDATED STOCK FOR %s              :            %.d\n", s->stock[j].product.name, s->stock[j].quantity);				
				printf("%s's UPDATED BUDGET              :        € %.2f\n", c->name, c->budget);
				printf("UPDATED SHOP BALANCE                 :       € %.2f\n", s->cash);
				printf("\n******************************************************\n");


        	}
		
        }
	
    }
   
}

// Live Customer entity - based this on the customer struct earlier in the doc
struct liveCustomer {
	double budget;                             // double budget accounts for numbers with decimal          
	struct ProductStock* liveShopList;     //  Pointer used for live shoppinglist as we don't know how long list will be 
	int index;                                 // keeps track of how many elements have been stored in customers shoppingList
};


struct liveCustomer createLiveCustomer(struct Shop s){
   
   struct liveCustomer liveorder; 						// Creates a liveorder struct
   liveorder.liveShopList = malloc(sizeof(struct ProductStock) * 30);


		printf("\n\nYOU HAVE NOW ENTERED THE INTERACTIVE SHOP MODE!!.\n");


		// User input - prompts the user to enter their detais - budget; product and quantity they would ike to purchase 

		printf("\nPLEASE ENTER YOUR NAME: ");
  		char *name = malloc(sizeof(char) * 50);
		scanf("%s", name);

		printf("PLEASE ENTER YOUR BUDGET: ");
		double budget;
		scanf("%lf", &budget);

	   // https://syntaxdb.com/ref/c/while
	   
  	liveorder.index = 0;    
	char continueOrder;

	 // this while loop will repeatidly loop until the customer enters n 
	 // I have put in this location as I only want the user name and budget once
	 // so it will ask product quantity repeatidly until customer says no 
	while(strcmp(&continueOrder, "n") != 0){		  
    	

		printf("PLEASE ENTER PRODUCT TO PURCHASE: "); 
		char* item = malloc(sizeof(char)*50);
    	scanf("\n%[^\n]%*c", item);
     
		printf("PLEASE ENTER THE QUANTITY TO PURCHASE: ");
		int qty;
		scanf("%d",&qty);
    
      // create a product and product stock with the information from the user input
		struct Product product = {item, getProduct(s, item).price};
		struct ProductStock listItem = {product, qty};

      // increment the index and put the stock item in to the stock array
		liveorder.liveShopList[liveorder.index] = listItem;
		liveorder.index++;
      

		printf("WOULD YOU LIKE TO PURCHASE ANYTHING ELSE? (y/n)");
		scanf("\n%s", &continueOrder);
   }
   return liveorder;
}



void reviewLiveOrder(struct Shop* s, struct liveCustomer lc){

    // Create the following variables:

    double orderCost = 0;             // Variable to  calculate the amount due for products stocked in shop
	double shortAmount = 0;           // Variable to calculate amount the customer is short when paying for items
	double shortStock = 0;            // Variable to calculate the shortage in stock in the shop vs what the customer ordered



    // Outer for loop which loops through customer shopping list 
    for(int i = 0; i <= lc.index; i++) {  

	// Calculate the total cost of the order by multiplying the item price by quantity customer wants
	orderCost = orderCost + (lc.liveShopList[i].quantity * lc.liveShopList[i].product.price);
		
	}	


	// Dilemma 1: Customer does not have enough money - customer budget is less than total cost of the products. Transaction will be cancelled as a result
	
	for(int i = 0; i < lc.index;i++){
		if(orderCost < lc.budget) {
				
			// Find the amount the customer is short by deducting the customer budget from the total order cost
			
			shortAmount = (orderCost - lc.budget); 
			printf("\n\n*****************************************************************************\n");
			printf("\nSORRY YOUR BUDGET DOES NOT COVER THE COST OF THE PRODUCTS YOU WISH TO PURCHASE!");
			printf("\nYOU ARE SHORT BY €%.2f \n", shortAmount);
			printf("\n*******************************************************************************\n");
			return;
			}

	}
	
	for(int i = 0; i < lc.index;i++){
		if (strcmp(findProduct(s, lc.liveShopList[i].product.name),"NULL")==0){
			
			printf("\nSORRY %s IS NOT STOCKED BY OUR SHOP AT THIS TIME!\n", lc.liveShopList[i].product.name);
			return;
		}
	}


// Dilemma 3: Shop stock is less than the quantity the customer wishes to order

    // Outer for loop which loops through customer shopping list 
    for(int i = 0; i < lc.index; i++) {

        // Inner for loop which loops through the shop stock
        for(int j = 0; j < s->index; j++){

            // Find if the item in the shopping list is in the shop  - this checks if the strings are the same 
            // https://c-for-dummies.com/blog/?p=2641
            if(strcmp(lc.liveShopList[i].product.name, s->stock[j].product.name)==0) {
            
            // if the shop stock quantity is less than the customer order quantity
                if(lc.liveShopList[i].quantity > s->stock[j].quantity) {
                    shortStock = (lc.liveShopList[i].quantity -  s->stock[j].quantity);

                    printf("\nSORRY OUR STORE ONLY HAS %d %s!\n", s->stock[j].quantity, lc.liveShopList[i].product.name);
				    printf("THERE ARE %f %d MISSING FROM YOUR ORDER\n", shortStock, lc.liveShopList[i].quantity);
				    printf("%s WILL NOT BE ADDED TO YOUR ORDER\n", lc.liveShopList[i].product.name);
					printf("PLEASE ADJUST ORDER ON FILE FOR %s AND REORDER\n",lc.liveShopList[i].product.name);
					return;
            }   

             // Update the shop stock quantity to reflect the customer purchasing products
            s->stock[j].quantity = s->stock[j].quantity - lc.liveShopList[i].quantity;
            
            // Update the shop balance to reflect the customer purchasing products
            s->cash = s->cash + (lc.liveShopList[i].product.price * lc.liveShopList[i].quantity);

            // Update the customer budget to reflect the purchase of products from the shop
            lc.budget = lc.budget - (lc.liveShopList[i].product.price * lc.liveShopList[i].quantity); 
	
		printf("\n%s HAS BEEN SUCCESSFULLY ORDERED\n", lc.liveShopList[i].product.name);
		printf("TOTAL COST OF %s                  :         € %.2f\n", lc.liveShopList[i].product.name, orderCost);
		printf("UPDATED STOCK FOR %s              :            %.d\n", s->stock[j].product.name, s->stock[j].quantity);				
		printf("UPDATED SHOP BALANCE                 :       € %.2f\n", s->cash);
		printf("\n******************************************************\n");

        	}
        }
    }
	
}


 
//Adapted from:
// https://www.w3resource.com/c-programming-exercises/conditional-statement/c-conditional-statement-exercises-25.php
void mainMenu(struct Shop s) {

	int choice = 0;
	char file;
    
	do {

		printf("\n\n**************************************************************************************\n");
		printf("***                                WELCOME TO THE SHOP                             ***\n");
		printf("**************************************************************************************\n");
		printf("***                                                                                ***\n");
		printf("***                                                                                ***\n");		
		printf("***                PLEASE SELECT FROM THE MENU OPTIONS BELOW :                     ***\n");	
		printf("***                                                                                ***\n");
		printf("***                                                                                ***\n");
		printf("***                ENTER  1 ==> VIEW SHOP CONTENTS                                 ***\n");
		printf("***                ENTER  2 ==> CUSTOMER ORDER FROM CSV FILE                       ***\n");            
		printf("***                ENTER  3 ==> CUSTOMER ORDER VIA LIVE MODE                       ***\n"); 
		printf("***                ENTER  4 ==> EXIT SHOP                                          ***\n"); 	
		printf("***                                                                                ***\n");
		printf("**************************************************************************************\n");
		printf("**************************************************************************************\n");
		printf("\n\nENTER YOUR CHOICE : ");
        scanf("%d",&choice);
		switch (choice)
		{
			case 1:{
					printShop(s);
					mainMenu(s);
			}

			case 2:{
					printf("ENTER CSV FILE NAME YOU WISH TO OPEN: \n");
      				scanf("%s", &file);
					struct Customer customer = customerOrder(s, &file); 
					printCustomer(customer);
                    reviewOrder(&s, &customer); 
					mainMenu(s);

			}

			case 3:{
					struct liveCustomer live = createLiveCustomer(s);
                    reviewLiveOrder(&s, live);
					printShop(s);
					mainMenu(s);
			}

			case 4:{
					printf("\n\n**************************************************************************************\n\n");
                    printf("**                 GOODBYE AND THANK YOU FOR SHOPPING WITH US!                      **\n\n");
                    printf("**************************************************************************************\n\n");		
	    		    exit(0);
			}

			default:	
					printf("\n\n**************************************************************************************\n\n");
                    printf("**                     INCORRECT SELECTION - PLEASE TRY AGAIN                         **\n\n");
                    printf("**************************************************************************************\n\n");		
					mainMenu(s);	

			
		}
	}

		// Continue until 99 is entered
	while (choice != 4);
	// The end
    return;
}
int main(void)
{  
   struct Shop shop = createAndStockShop("stock.csv");
   mainMenu(shop);
   return 0;
}


/*
References:
https://www.sanfoundry.com/c-program-inventory-items-store/
https://en.wikipedia.org/wiki/Pointer_(computer_programming)
http://www.tulane.edu/~mpuljic/cpp/savitch/chapter15.pdf
https://github.com/Roisin-Fallon/Multi-Paradigm-Programming-Assignment/blob/master/c-shop/shop.c
https://github.com/shkyler/gmit-mpp-shop