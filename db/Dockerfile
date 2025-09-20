# db/Dockerfile

# Use the official postgres image as the base
FROM postgres:13

# Copy the init.sql script into the container.
# The official postgres image automatically runs any scripts in this directory.
COPY ./init.sql /docker-entrypoint-initdb.d/