/*
* Copyright (C) 2013 Kotori <https://github.com/kotori>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*! \file database.h
    \brief SQLite3 C++ Wrapper Header

SQLite3 C++ Wrapper Header. Contains the framework for the Database class.
*/

#ifndef DATABASE_H
#define DATABASE_H

extern std::string DEFAULT_LOC; /*!< String representing the path to the db file. */

#define ACTIVE_LOGGING false /*!< Logging flag switch. */

#include <sqlite3.h>

/*! \class Database
    \brief Database class

Database class provides SQLite3 I/O functionality.
*/
class Database
{
public:
    /*! \fn Database()
        \brief Database class constructor

    Database class constructor. Takes no arguments. Sets db_loc_ to DEFAULT_LOC
    */
    Database();

    /*! \fn Database(std::string loc)
        \brief Database class constructor. With override.
        \param loc string representing the location of the SQLite3 database on the filesystem.

    Database class constructor. This function will allow you to specify the location of the database.
    */
    Database(std::string loc);

    /*! \fn ~Database()
    \brief Database class destructor.

    Database class destructor. Nothing special here, should not be called directly.
    */
    ~Database();

    /*! \fn bool SelectInt(int &refVal, const char *sql, ...)
    \brief Select a single integer from the database.
    \param refVal Integer reference containing the value where are selecting.
    \param sql SQL string containing the select integer statement.
    \return bool False on success, True on error.

    Retrieves a single integer from the database. If an error is thrown during the select process,
    this function will return true. Otherwise false.
    */
    bool SelectInt(int &refVal, const char *sql, ...);

    /*! \fn bool SelectStr(std::string &refVal, const char *sql, ...)
    \brief Select a single string from the database.
    \param refVal std::string reference containing the value where are selecting.
    \param sql SQL string containing the select string statement.
    \return bool False on success, True on error.

    Retrieves a single std::string from the database. If an error is thrown during the select process,
    this function will return true. Otherwise false.
    */
    bool SelectStr(std::string &refVal, const char *sql, ...);

    /*! \fn bool SelectChar(char* refVal, const char *sql, ...)
    \brief Select a single string from the database.
    \param refVal char string array containing the value where are selecting.
    \param sql SQL string containing the select string statement.
    \return bool False on success, True on error.

    Retrieves a single char string array from the database. If an error is thrown during the select process,
    this function will return true. Otherwise false.
    */
    bool SelectChar(char* refVal, const char *sql, ...);

    /*! \fn char* Format(const char *msg, ...)
    \brief Formats a string with printf style arguments.
    \param msg string to process containing printf arguments.
    \return A printf style formatted string.

    Formats a string with printf style arguments "%s,%i" and returns a string with the variables embedded.
    */
    char* Format(const char *msg, ...);

    /*! \fn bool Exec(const char *sql, ...)
    \brief Executes a SQL statement that does NOT require a returned value.
    \param sql SQL string containing the code you want to execute.
    \return bool False on success, True on error.

    Executes a SQL command that does not require a returned value, example: CREATE, DROP, UPDATE, INSERT. If an error
    is thrown during the process, this function will return true. Otherwise false.
    */
    bool Exec(const char *sql, ...);

private:

    /*! \fn void Common()
    \brief Runs routines that are common among the constructors.

    This function is called by the different constructors to maximize code reusage.
    */
    void Common();

    /*! \fn void Close()
    \brief Closes an open connection to the SQLite3 database.

    This function should safely close an open connection to /a db_ SQLite3 database.
    */
    void Close();

    /*! \fn bool Open()
    \brief Opens a connection to a SQLite3 database.

    This function should safely opens a connection to /a db_ SQLite3 database.
    */
    bool Open();

    bool m_isOpen;			/*!< Database is open flag, true if open, false if closed. */
    std::string m_dbLoc;	/*!< Internal variable for storing the location of the db. */
    sqlite3 *m_db;			/*!< SQLite3 database connection handle. */
};

#endif // DATABASE_H
