-- Drop the table if it already exists to ensure a clean start
DROP TABLE IF EXISTS products;

-- Create the products table
CREATE TABLE products (
                          id SERIAL PRIMARY KEY,
                          name VARCHAR(255) NOT NULL,
                          price DECIMAL(10, 2) NOT NULL,
                          created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- Insert initial data into the products table
INSERT INTO products (name, price) VALUES
                                       ('Laptop', 1200.50),
                                       ('Mouse', 25.00),
                                       ('Keyboard', 75.25);

-- Create an index to improve query performance on the name column
CREATE INDEX idx_products_name ON products (name);