# stocks.console

This console application allows:
1) Checking Alpaca stock account.
2) Checking indicator against a given symbol.

# run postgres

docker compose up --build

docker run --name my-postgres -e POSTGRES_PASSWORD=lymm -p 5432:5432 -d postgres:13


https://medium.com/better-programming/connect-from-local-machine-to-postgresql-docker-container-f785f00461a7