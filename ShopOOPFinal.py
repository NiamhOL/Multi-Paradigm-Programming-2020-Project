import csv

class Product: # Simple class containing a name and a price.

    def __init__(self, name, price=0): 
        self.price = price
    
    def __repr__(self):
        return f'NAME: {self.name} PRICE: {self.price}' # Returns name and the price

class ProductStock: # has a product and a quantity 
    
    def __init__(self, product, quantity):
        self.product = product
        self.quantity = quantity
    
    def name(self): # Name of product
        return self.product.name
    
    def unit_price(self): # Returns price of product
        return self.product.price

    def cost(self): 
        return self.unit_price() * self.quantity
        
    def __repr__(self): # Returns product and quantity.
        return f"{self.product} QUANTITY: {self.quantity}" # calls product class.**

class Customer:

    def __init__(self, path): # method, called constructor, called when object created from class.
        self.shopping_list = []
        with open(path) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            first_row = next(csv_reader) # pulls in info about itself from csv
            self.name = first_row[0] # name from first row
            self.budget = float(first_row[1]) 
            for row in csv_reader: 
                name = row[0]
                quantity = float(row[1])
                p = Product(name)
                ps = ProductStock(p, quantity)
                self.shopping_list.append(ps) # stock that are being added to a shopping list,
            
                
    def calculate_costs(self, price_list): # method passes in stock array, 
        for shop_item in price_list: #  for loop for all stock, 
            for list_item in self.shopping_list: # then an inner loop, for every item in shop, check it against what I want to buy.
                if (list_item.name() == shop_item.name()): # if 2 names are same, 
                    list_item.product.price = shop_item.unit_price() # save price of shop items 
    
    def order_cost(self): # 
        cost = 0
        
        for list_item in self.shopping_list: # for every item in shooping list,
            cost += list_item.cost() 
        
        return cost # return total.
    
    def __repr__(self):
        
        str = f"{self.name} wants to buy" # gets name of customer
        for item in self.shopping_list: # iterate through for loop in shopping list
            cost = item.cost() # call method to calculate cost of item
            str += f"\n{item}" 
            if (cost == 0): # if cost = 0, 
                str += f" {self.name} does not know the cost" # name of customer and prints
            else:
                str += f" COST: {cost}"
                
        str += f"\nThe cost would be: {self.order_cost()}, budget  {self.budget - self.order_cost()} remainaing"
        return str 
        
class Shop:
    
    def __init__(self, path): # Constructer, path to csv file
        self.stock = [] # Create blank array to record stock
        with open(path) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            first_row = next(csv_reader)
            self.cash = float(first_row[0]) # Sets cash value
            for row in csv_reader:
                p = Product(row[0], float(row[1])) # Create products 
                ps = ProductStock(p, float(row[2])) # Create productStock
                self.stock.append(ps) # 
    
    def __repr__(self): # repr method 
        str = "" # Creates string 
        str += f'Shop has {self.cash} in cash\n' # Appends info to that string.
        for item in self.stock: # Loops through all the product stock in array stock,
            str += f"{item}\n" # Returns that info about item, line by line. 
        return str # returns string about everything in shop

s = Shop("stock.csv")
#print(s)

c = Customer("customer.csv")
c.calculate_costs(s.stock)
print(c)