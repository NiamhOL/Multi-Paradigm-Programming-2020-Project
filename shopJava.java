package shop_java;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Shop {
	private double cash;
	private static ArrayList<ProductStock> stock;

	public Shop(String fileName) {
		stock = new ArrayList<>();
		List<String> lines = Collections.emptyList();
		try {
			lines = Files.readAllLines(Paths.get(fileName), StandardCharsets.UTF_8);
			// System.out.println(lines.get(0));
			String[] vals = lines.get(0).split(",");
			cash = Double.parseDouble(vals[1]);
			// cash = Double.parseDouble(lines.get(0));
			// i am removing at index 0 as it is the only one treated differently
			lines.remove(0);
			for (String line : lines) {
				// System.out.println(line);
				String[] arr = line.split(",");
				String name = arr[0];
				double price = Double.parseDouble(arr[1]);
				int quantity = Integer.parseInt(arr[2].trim());
				Product p = new Product(name, price);
				ProductStock s = new ProductStock(p, quantity);
				stock.add(s);
			}
		}

		catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void setCash(double cash) {
		this.cash = cash;
	}

	public double getCash() {
		return cash;
	}

	public ArrayList<ProductStock> getStock() {
		return stock;
	}

	public String findProdInfo(String findme, int oQuantity){
		/**
		 * <h2>findProductInfo</h2>
		 * 
		 * inputs: String findme, int Quantity
		 * 
		 * findme - the item name in the shop that should be searched for
		 * Quantity - the quantity that is required on ythe order
		 * 
		 * Return Values:
		 * 
		 * NONE or the matched item including name, quaantity and price as well as a comment if
		 * stock was less than match. 
		 */
		String result=findme;
		int deficit=0;
		String msg="";// populate if stock is short
		for (ProductStock stockItem : Shop.stock) {
			if (stockItem.getProduct().getName().equalsIgnoreCase(findme)) {
				result = "";
				String pName = stockItem.getProduct().getName();
				int pQuantity = stockItem.getQuantity();
				double pPrice = stockItem.getProduct().getPrice();
				System.out.println(result);
				if (oQuantity > pQuantity) {// more things are ordered than available
					deficit = oQuantity - pQuantity;
					oQuantity = pQuantity; // then set the order quantity to the available quantity
				}
				if (deficit > 0) {
					msg = "," + deficit + " short";
				}
				double total = oQuantity * pPrice;
				stockItem.setQuantity(pQuantity - oQuantity);// subtract the order quantity from the stock
				result = "" + pName + "," + oQuantity + "," + String.format("%.2f", pPrice) + "," + String.format("%.2f", total)
						+ msg;
				// return result;
			}
		}
		return result;
	}

	public static double printItemDetails(String sItem, boolean detail) {
		/**
		 * <h2>printItemDetails</h2>
		 * 
		 * Inputs: The item details returned by findProdInfo and a boolean flag for details.
		 * 
		 * When the boolean is true the function outputs a list with labels and values otherwise it 
		 * prints single line with sum and total.
		 *  
		 */
		String[] itenNames = { "Item Name", "Order Quantity", "Unit Price", "Total", "Remark" };
		String[] items = sItem.split(",");
		double lineTotal = 0.0;
		if (detail==true) {
			for (int i = 0; i < items.length; i++) {
				System.out.println(String.format("%14s: ",itenNames[i]) + items[i]);
			}	
		}
		else{
			if (items.length==1){ 
				System.out.printf("\n%-12s - not stocked", sItem);
			}
			else if (items.length==4) {
				System.out.printf("%-12s x %3s @ %5s = %5s", items[0], items[1], items[2], items[3]);
				lineTotal = Double.parseDouble(items[3].strip());	
			}
			else if (items.length==5){
				System.out.printf("%-12s x %3s @ %5s = %5s - %s", items[0], items[1], items[2], items[3], items[4]);
				lineTotal = Double.parseDouble(items[3].strip());
			}
		}
		return lineTotal;
	}

	public void printInvenrtory(){
		String sep="=======================\n";
		System.out.printf("\n%s    SHOP INVENTORY \n",sep);
		System.out.printf("%s%-16s %.2f\n", sep, "CASH", cash);
		System.out.printf("%s%-14s %-5s %s\n%s",sep ,"Item","€","Q",sep);
		for (ProductStock productStock : stock) {
			int q = productStock.getQuantity();
			String n = productStock.getProduct().getName();
			double p = productStock.getProduct().getPrice();
			System.out.printf("%-14s %.2f %3d\n",n,p,q);
		}
	}
	
	@Override
	public String toString() {
		return "\nShop [\n  cash=" + cash + ", \n  stock=" + stock + "\n]";
	}

	public static void main(String[] args) {
		Shop shop = new Shop("src/shop_java/stock.csv");
		//System.out.println(shop);
		shop.printInvenrtory();
	}
}