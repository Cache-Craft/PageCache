import os
import mysql.connector
from mysql.connector import Error


def get_connection():
    """Create and return a MySQL database connection."""
    try:
        connection = mysql.connector.connect(
            host=os.getenv("DB_HOST", "localhost"),
            port=int(os.getenv("DB_PORT", "3306")),
            user=os.getenv("DB_USER", "root"),
            password=os.getenv("DB_PASSWORD", ""),
            database=os.getenv("DB_NAME", "pagecache_db"),
            connection_timeout=5,
        )

        if connection.is_connected():
            return connection

        connection.close()
        raise RuntimeError("MySQL connection could not be established.")

    except Error as exc:
        raise RuntimeError(f"MySQL connection failed: {exc}") from exc


def close_connection(connection):
    """Safely close a connection if it is open."""
    if connection is not None and connection.is_connected():
        connection.close()
