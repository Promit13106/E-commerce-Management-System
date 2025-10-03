#include <iostream>   /// For input and output operations
#include <fstream>    /// For file handling (reading/writing user, product, cart data)
#include <string>     /// For using the string class (username, password, product names)
#include <vector>     /// For dynamic arrays (store products, users, cart items)
#include <iomanip>    /// For formatted output (bills, product tables)

using namespace std;


/// Data Structures
/// Structure for product information
struct Product
{
    string Product_name;   /// Name of the product
    double Price;          /// Price of the product
    long Product_stock;    /// Available stock of the product
};

/// Structure for items in a customer's cart
struct Cart_Item
{
    string name;           /// Product name
    double price;          /// Price of single unit
    int quantity;          /// Quantity purchased
};

/// Structure for user account information
struct UserRecord
{
    string username;       /// Login username
    string password;       /// Login password
    string role;           /// User role: "Admin" or "Customer"
};


/// Customer Class

class Customer
{
private:
    string username;       /// Customer's username
    string password;       /// Customer's password
    vector<Cart_Item> cart;/// Each customer has their own shopping cart

    /// Load customer's cart data from file (cart_username.txt)
    void load_cart()
    {
        ifstream fin("cart_" + username + ".txt");
        if (!fin) return; /// If no file exists, cart is empty

        Cart_Item item;
        while (getline(fin, item.name) && fin >> item.price >> item.quantity)
        {
            cart.push_back(item);
            fin.ignore(); /// Ignore newline character
        }
        fin.close();
    }

    /// Save customer's current cart data to file
    void save_cart()
    {
        ofstream fout("cart_" + username + ".txt");
        for (auto &item : cart)
        {
            fout << item.name << endl
                 << item.price << " " << item.quantity << endl;
        }
        fout.close();
    }

    /// Display items in the cart
    void view_cart()
    {
        if (cart.empty())
        {
            cout << "Your cart is empty." << endl;
            return;
        }
        cout << "=======Your Cart=======" << endl;
        for (int i = 0; i < cart.size(); i++)
        {
            cout << i + 1 << ". " << cart[i].name
                 << " | Price: " << cart[i].price << " BDT"
                 << " | Qty: " << cart[i].quantity << endl;
        }
    }

public:
    /// Constructor initializes customer and loads their cart
    Customer(string u = "", string p = "") : username(u), password(p)
    {
        load_cart();
    }

    /// Show customer information (basic details)
    void show_customer_info()
    {
        cout << "Customer Username: " << username << endl;
    }

    /// Checkout process: generate bill, update stock, clear cart
    void checkout(vector<Product> &products)
    {
        if (cart.empty())
        {
            cout << "Your cart is empty. Nothing to checkout." << endl;
            return;
        }

        double total = 0;
        cout << "=======Checkout=======" << endl;

        /// Print bill details on screen
        for (auto &item : cart)
        {
            double subtotal = item.price * item.quantity;
            cout << item.name << " | Price: " << item.price << " BDT"
                 << " | Qty: " << item.quantity
                 << " | Subtotal: " << subtotal << " BDT" << endl;
            total += subtotal;
        }
        cout << "Total Amount: " << total << " BDT" << endl;
        cout << "Thank you for your purchase! Your products will be delivered soon." << endl;

        /// Save bill to a file (bill_username.txt)
        ofstream fout("bill_" + username + ".txt", ios::app);
        fout << "=================New Bill=================" << endl;
        for (auto &item : cart)
        {
            fout << item.name << " | Price: " << item.price << " BDT"
                 << " | Qty: " << item.quantity
                 << " | Subtotal: " << item.price * item.quantity << " BDT" << endl;
        }
        fout << "Total Amount: " << total << " BDT" << endl;
        fout << "==================================================" << endl << endl;
        fout.close();

        /// Clear cart after checkout
        cart.clear();
        save_cart();

        /// Update product stock in products.txt
        ofstream pfout("products.txt");
        for (auto &p : products)
        {
            pfout << p.Product_name << endl
                  << p.Price << " " << p.Product_stock << endl;
        }
        pfout.close();
    }

    /// Customer menu options
    void Customer_menu(vector<Product> &products)
    {
        int user_choice;
        do
        {
            cout << " =======Customer Menu=======" << endl;
            cout << "1. View Products" << endl;
            cout << "2. Add to Cart" << endl;
            cout << "3. View Cart" << endl;
            cout << "4. Checkout" << endl;
            cout << "5. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> user_choice;

            switch (user_choice)
            {
            case 1: /// View available products
                if (products.empty())
                {
                    cout << "No products available currently." << endl;
                }
                else
                {
                    cout << "=======Products List=======" << endl;
                    for (int i = 0; i < products.size(); i++)
                        cout << i + 1 << ". " << products[i].Product_name
                             << " | Price: " << products[i].Price << " BDT"
                             << " | Stock: " << products[i].Product_stock << endl;
                }
                break;

            case 2: /// Add product to cart
            {
                if (products.empty())
                {
                    cout << "No products available to add." << endl;
                    break;
                }
                int id, quantity;
                cout << "Enter product ID: ";
                cin >> id;
                cout << "Enter quantity: ";
                cin >> quantity;

                if (id <= 0 || id > products.size() || quantity <= 0)
                {
                    cout << "Invalid input!" << endl;
                }
                else if (products[id - 1].Product_stock < quantity)
                {
                    cout << "Only " << products[id - 1].Product_stock << " items in stock." << endl;
                }
                else
                {
                    cart.push_back({products[id - 1].Product_name, products[id - 1].Price, quantity});
                    products[id - 1].Product_stock -= quantity;
                    cout << "Added to your cart successfully!" << endl;
                    save_cart();
                }
                break;
            }

            case 3: /// View current cart
                view_cart();
                break;

            case 4: /// Checkout
                checkout(products);
                break;

            case 5: /// Logout
                cout << "Logging out... Thanks for visiting our shop." << endl;
                break;

            default:
                cout << "Invalid choice! Try again." << endl;
            }

        }
        while (user_choice != 5);
    }
};

/// Admin Class

class Admin
{
private:
    string admin_username;    /// Admin username
    string admin_password;    /// Admin password

    /// Save updated products list to file
    void save_products(vector<Product> &products)
    {
        ofstream fout("products.txt");
        for (auto &p : products)
            fout << p.Product_name << endl
                 << p.Price << " " << p.Product_stock << endl;
        fout.close();
    }

public:
    /// Constructor initializes admin
    Admin(string u = "", string p = "") : admin_username(u), admin_password(p) {}

    /// Admin menu options
    void Admin_menu(vector<Product> &products)
    {
        int choice;
        do
        {
            cout << "=======Admin Menu=======" << endl;
            cout << "1. Add Product" << endl;
            cout << "2. View Products" << endl;
            cout << "3. Edit Product" << endl;
            cout << "4. Remove Product" << endl;
            cout << "5. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1: /// Add new product
            {
                Product p;
                cout << "Enter product name: ";
                cin.ignore();
                getline(cin, p.Product_name);
                cout << "Enter price (in BDT): ";
                cin >> p.Price;
                cout << "Enter stock: ";
                cin >> p.Product_stock;
                products.push_back(p);
                save_products(products);
                cout << "Product added successfully!" << endl;
                break;
            }

            case 2: /// View products
                if (products.empty())
                {
                    cout << "No products available." << endl;
                }
                else
                {
                    cout << "=======Products List=======" << endl;
                    for (int i = 0; i < products.size(); i++)
                        cout << i + 1 << ". " << products[i].Product_name
                             << " | Price: " << products[i].Price << " BDT"
                             << " | Stock: " << products[i].Product_stock << endl;
                }
                break;

            case 3: /// Edit product details
            {
                int id;
                cout << "Enter product ID to edit: ";
                cin >> id;
                if (id <= 0 || id > products.size())
                {
                    cout << "Invalid ID!" << endl;
                    break;
                }
                cin.ignore();
                cout << "Enter new name: ";
                getline(cin, products[id - 1].Product_name);
                cout << "Enter new price: ";
                cin >> products[id - 1].Price;
                cout << "Enter new stock: ";
                cin >> products[id - 1].Product_stock;
                save_products(products);
                cout << "Product updated successfully!" << endl;
                break;
            }

            case 4: /// Remove product
            {
                int id;
                cout << "Enter product ID to remove: ";
                cin >> id;
                if (id <= 0 || id > products.size())
                {
                    cout << "Invalid ID!" << endl;
                    break;
                }
                products.erase(products.begin() + id - 1);
                save_products(products);
                cout << "Product removed successfully!" << endl;
                break;
            }

            case 5: /// Logout
                cout << "Logging out..." << endl;
                break;

            default:
                cout << "Invalid choice!" << endl;
            }

        }
        while (choice != 5);
    }
};

-
/// Global Functions

vector<UserRecord> users; /// Global list of users (admins + customers)

/// Load users from files (admins.txt and customers.txt)
void load_users()
{
    users.clear();  /// Reset user list

    ifstream fin_admin("admins.txt");
    if (fin_admin)
    {
        UserRecord u;
        while (fin_admin >> u.username >> u.password >> u.role)
            users.push_back(u);
        fin_admin.close();
    }

    ifstream fin_customer("customers.txt");
    if (fin_customer)
    {
        UserRecord u;
        while (fin_customer >> u.username >> u.password >> u.role)
            users.push_back(u);
        fin_customer.close();
    }
}

/// Load products from products.txt
void load_products(vector<Product> &products)
{
    ifstream fin("products.txt");
    if (!fin) return; /// If no file exists, return

    Product p;
    while (getline(fin, p.Product_name) && fin >> p.Price >> p.Product_stock)
    {
        products.push_back(p);
        fin.ignore();
    }
    fin.close();
}

/// Register a new user (admin or customer)
void register_user()
{
    string u, p, role;
    cout << "=====Registration======" << endl;
    cout << "Enter username: ";
    cin >> u;

    /// Check if username already exists
    for (auto &user : users)
    {
        if (user.username == u)
        {
            cout << "Username already exists! Try another." << endl;
            return;
        }
    }

    cout << "Enter password: ";
    cin >> p;
    cout << "Enter role (admin/customer): ";
    cin >> role;

    if (role != "admin" && role != "customer")
    {
        cout << "Invalid role. Try again." << endl;
        return;
    }

    /// Add to user list and save in appropriate file
    users.push_back({u, p, role});

    if (role == "admin")
    {
        ofstream fout("admins.txt", ios::app);
        fout << u << " " << p << " " << role << endl;
        fout.close();
    }
    else
    {
        ofstream fout("customers.txt", ios::app);
        fout << u << " " << p << " " << role << endl;
        fout.close();
    }

    cout << "Registration successful!" << endl;
}

/// Login function
bool login(string &role, string &username)
{
    string u, p;
    cout << "\n--- Login ---" << endl;
    cout << "Enter username: ";
    cin >> u;
    cout << "Enter password: ";
    cin >> p;

    /// Check against loaded users
    for (auto &user : users)
    {
        if (user.username == u && user.password == p)
        {
            role = user.role;
            username = user.username;
            return true;
        }
    }
    return false;
}

/// Main Function

int main()
{
    vector<Product> products;  /// Product list
    load_users();              /// Load user data
    load_products(products);   /// Load product data

    string role, username;
    int choice;

    do
    {
        cout << "======Main Menu======" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: /// Register user
            register_user();
            break;

        case 2: /// Login and show respective menu
            if (login(role, username))
            {
                cout << "Logged in successfully! Role: " << role << endl;
                if (role == "admin")
                {
                    Admin admin(username, "");
                    admin.Admin_menu(products);
                }
                else
                {
                    Customer customer(username, "");
                    customer.Customer_menu(products);
                }
            }
            else
            {
                cout << "Invalid username or password!" << endl;
            }
            break;

        case 3: /// Exit program
            cout << "Exiting program. Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice! Try again." << endl;
        }
    }
    while (choice != 3);

    return 0;
}

