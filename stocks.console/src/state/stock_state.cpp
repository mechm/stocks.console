#include "stock_state.h"

#include <iostream>
#include <string>
#include <pqxx/pqxx>

void Insert() {
    try {
        const std::string conn_string ="dbname=stocks user=sa password=lymm host=localhost port=5432";

        // Establish a connection
        pqxx::connection conn(conn_string);
        std::cout << "Connected to the database." << std::endl;

        // Use a transaction object to execute a query
        pqxx::work txn(conn);

        // Prepare a statement for a single product insertion.
        // The placeholders ($1, $2) are crucial for security.
        conn.prepare(
            "insert_product",
            "INSERT INTO products (name, price) VALUES ($1, $2);"
        );

        // Product data to be inserted
        std::string new_product_name = "Monitor";
        double new_product_price = 199.99;

        // Execute the prepared statement with the product data as parameters
        txn.exec_prepared("insert_product", new_product_name, new_product_price);

        // Commit the transaction
        txn.commit();

        std::cout << "Product '" << new_product_name << "' inserted successfully." << std::endl;

    } catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}