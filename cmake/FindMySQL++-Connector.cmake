# - Find MySQL++ Connector
# Find the MySQL++ Connector includes and client library
# This module defines
#  MYSQLPP_CONNECTOR_INCLUDE_DIR, where to find mysql_connection.h
#  MYSQLPP_CONNECTOR_LIBRARIES, the libraries needed to use MySQL++ Connector.
#  MYSQLPP_CONNECTOR_FOUND, If false, do not try to use MySQL++ Connector.

FIND_PATH(MYSQLPP_CONNECTOR_INCLUDE_DIR mysql_connection.h
   /usr/local/include/cppconn
)

FIND_LIBRARY(MYSQLPP_CONNECTOR_LIBRARIES NAMES mysqlcppconn
   PATHS
   /usr/local/lib
)

if(MYSQLPP_CONNECTOR_INCLUDE_DIR AND MYSQLPP_CONNECTOR_LIBRARIES)
   set(MYSQLPP_FOUND TRUE)
   message(STATUS "Found MySQL++: ${MYSQLPP_CONNECTOR_INCLUDE_DIR}, ${MYSQLPP_CONNECTOR_LIBRARIES}")
else(MYSQLPP_CONNECTOR_INCLUDE_DIR AND MYSQLPP_CONNECTOR_LIBRARIES)
   set(MYSQLPP_FOUND FALSE)
   message(STATUS "MySQL++ was not found.")
endif(MYSQLPP_CONNECTOR_INCLUDE_DIR AND MYSQLPP_CONNECTOR_LIBRARIES)

mark_as_advanced(MYSQLPP_CONNECTOR_INCLUDE_DIR MYSQLPP_CONNECTOR_LIBRARIES)
