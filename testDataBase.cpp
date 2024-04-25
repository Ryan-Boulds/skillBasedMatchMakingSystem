#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <vector>
// My libraries
#include "createPlayerData.h"

// Global variables for database connection
SQLHANDLE sqlconnectionhandle;
SQLHANDLE sqlenvhandle;

void show_error(unsigned int handletype, const SQLHANDLE& handle)
{
    SQLWCHAR sqlstate[1024];
    SQLWCHAR message[1024];
    if (SQL_SUCCESS == SQLGetDiagRecW(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
        std::wcerr << L"SQL Error: " << message << L"\n";
}

int testDatabase()
{
    SQLHANDLE sqlstatementhandle;
    SQLRETURN retcode;

    // Allocate environment handle
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
        return -1;

    // Set ODBC version attribute
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlenvhandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        return -1;

    // Allocate connection handle
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
        return -1;

    // Connect to the DSN with the specified credentials
    SQLWCHAR dsn[] = L"playerstatsGlobal";
    SQLWCHAR uid[] = L"System";
    SQLWCHAR pwd[] = L"Passw0rd";
    retcode = SQLConnectW(sqlconnectionhandle, dsn, SQL_NTS, uid, SQL_NTS, pwd, SQL_NTS);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
    {
        SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle);

        // Execute a simple SQL statement to fetch the current date
        SQLWCHAR query[] = L"SELECT SYSDATE FROM DUAL";
        SQLExecDirectW(sqlstatementhandle, query, SQL_NTS);

        // Fetch and print the result
        SQLWCHAR date[20] = { 0 }; // Initialize date variable
        SQLLEN indicator = 0;      // Initialize indicator variable
        SQLBindCol(sqlstatementhandle, 1, SQL_C_WCHAR, date, sizeof(date), &indicator);
        while (SQLFetch(sqlstatementhandle) == SQL_SUCCESS)
        {
            if (indicator != SQL_NULL_DATA)
                std::wcout << L"Current Date: " << date << std::endl;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle);
        SQLDisconnect(sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);
    }
    else
    {
        show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
    }

    return 0;
}

int printDatabase()
{
    SQLHANDLE sqlstatementhandle;
    SQLRETURN retcode;

    // Allocate environment handle
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
        return -1;

    // Set ODBC version attribute
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlenvhandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        return -1;

    // Allocate connection handle
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
        return -1;

    // Connect to the DSN with the specified credentials
    SQLWCHAR dsn[] = L"playerstatsGlobal";
    SQLWCHAR uid[] = L"System";
    SQLWCHAR pwd[] = L"Passw0rd";
    retcode = SQLConnectW(sqlconnectionhandle, dsn, SQL_NTS, uid, SQL_NTS, pwd, SQL_NTS);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
    {
        SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle);

        // Execute a SELECT statement to retrieve data from the database
        SQLWCHAR query[] = L"SELECT * FROM Players"; // Change the query according to your table structure
        retcode = SQLExecDirectW(sqlstatementhandle, query, SQL_NTS);

        // Fetch and print the result
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
        {
            SQLINTEGER playerId;
            SQLINTEGER mmr;
            SQLINTEGER mmrChangeLevel;
            SQLINTEGER winStreak;
            SQLINTEGER loseStreak;
            SQLINTEGER gamesPlayed;
            SQLLEN indicator;

            // Bind columns to variables
            SQLBindCol(sqlstatementhandle, 1, SQL_C_LONG, &playerId, 0, &indicator);
            SQLBindCol(sqlstatementhandle, 2, SQL_C_LONG, &mmr, 0, &indicator);
            SQLBindCol(sqlstatementhandle, 3, SQL_C_LONG, &mmrChangeLevel, 0, &indicator);
            SQLBindCol(sqlstatementhandle, 4, SQL_C_LONG, &winStreak, 0, &indicator);
            SQLBindCol(sqlstatementhandle, 5, SQL_C_LONG, &loseStreak, 0, &indicator);
            SQLBindCol(sqlstatementhandle, 6, SQL_C_LONG, &gamesPlayed, 0, &indicator);

            while (SQLFetch(sqlstatementhandle) == SQL_SUCCESS)
            {
                std::cout << "PlayerID: " << playerId << ", MMR: " << mmr << ", MMRChangeLevel: " << mmrChangeLevel
                    << ", WinStreak: " << winStreak << ", LoseStreak: " << loseStreak << ", GamesPlayed: " << gamesPlayed << std::endl;
            }
        }
        else
        {
            show_error(SQL_HANDLE_STMT, sqlstatementhandle);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle);
        SQLDisconnect(sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);
    }
    else
    {
        show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
    }

    return 0;
}


int addPlayerTestData() {
    // Establish connection to the database
    SQLRETURN retcode;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle)) {
        show_error(SQL_HANDLE_ENV, sqlenvhandle);
        return -1;
    }

    if (SQL_SUCCESS != SQLSetEnvAttr(sqlenvhandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) {
        show_error(SQL_HANDLE_ENV, sqlenvhandle);
        return -1;
    }

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle)) {
        show_error(SQL_HANDLE_ENV, sqlconnectionhandle);
        return -1;
    }

    SQLWCHAR dsn[] = L"playerstatsGlobal";
    SQLWCHAR uid[] = L"System";
    SQLWCHAR pwd[] = L"Passw0rd";
    retcode = SQLConnectW(sqlconnectionhandle, dsn, SQL_NTS, uid, SQL_NTS, pwd, SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);
        return -1;
    }

    // Create the "Players" table
    std::wstring createTableQuery = L"CREATE TABLE Players (PlayerID INT, MMR INT, MMRChangeLevel INT, winStreak INT, loseStreak INT, gamesPlayed INT)";
    retcode = SQLExecDirectW(sqlconnectionhandle, (SQLWCHAR*)createTableQuery.c_str(), SQL_NTS);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        show_error(SQL_HANDLE_STMT, sqlconnectionhandle);
        SQLDisconnect(sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);
        return -1;
    }

    // Get test data for players
    int numOfSquadsToCreate = 10; // Choose the number of squads to create
    vector<vector<int>> playersData = playersTestData(numOfSquadsToCreate);

    // Prepare SQL INSERT statement
    std::wstring insertStatement = L"INSERT INTO Players (PlayerID, MMR, MMRChangeLevel, winStreak, loseStreak, gamesPlayed) VALUES (?, ?, ?, ?, ?, ?)";

    // Execute INSERT statement for each player's data
    SQLHANDLE hstmt;
    for (const auto& player : playersData) {
        SQLINTEGER paramValues[6] = { 0 }; // Initialize parameter values
        for (int i = 0; i < 6; ++i) {
            paramValues[i] = (i < player.size()) ? player[i] : 0; // Use 0 if player data is not available
        }

        // Allocate statement handle
        if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &hstmt)) {
            show_error(SQL_HANDLE_STMT, hstmt);
            continue; // Continue to next player data
        }

        // Bind parameters
        for (int i = 0; i < 6; ++i) {
            retcode = SQLBindParameter(hstmt, i + 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &paramValues[i], 0, NULL);
            if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
                show_error(SQL_HANDLE_STMT, hstmt);
                SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
                continue; // Continue to next player data
            }
        }
        // Execute the statement
        std::wstring wideInsertStatement(insertStatement.begin(), insertStatement.end());
        retcode = SQLExecDirectW(hstmt, &wideInsertStatement[0], SQL_NTS);
        if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
            show_error(SQL_HANDLE_STMT, hstmt);
        }


        // Free statement handle
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }

    // Disconnect from the database
    SQLDisconnect(sqlconnectionhandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);

    return 0;
}



int dropPlayerTable()
{
    SQLHANDLE sqlstatementhandle;
    SQLRETURN retcode;

    // Allocate environment handle
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
        return -1;

    // Set ODBC version attribute
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlenvhandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        return -1;

    // Allocate connection handle
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
        return -1;

    // Connect to the DSN with the specified credentials
    SQLWCHAR dsn[] = L"playerstatsGlobal";
    SQLWCHAR uid[] = L"System";
    SQLWCHAR pwd[] = L"Passw0rd";
    retcode = SQLConnectW(sqlconnectionhandle, dsn, SQL_NTS, uid, SQL_NTS, pwd, SQL_NTS);

    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
    {
        SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle);

        // Execute a DROP TABLE statement to drop the Players table
        SQLWCHAR query[] = L"DROP TABLE Players"; // Change the table name accordingly
        retcode = SQLExecDirectW(sqlstatementhandle, query, SQL_NTS);

        if (retcode != SQL_SUCCESS)
        {
            show_error(SQL_HANDLE_STMT, sqlstatementhandle);
            return -1;
        }

        SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle);
        SQLDisconnect(sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);
    }
    else
    {
        show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
        return -1;
    }

    return 0;
}

int testMain() {
    int input;
    std::cout << "Select one: ";
    std::cin >> input;

    if (input == 0) {
        testDatabase();
    }
    else if (input == 1) {
        printDatabase();
    }
    else if (input == 2) {
        addPlayerTestData();
    }
    else if (input == 3) {
        dropPlayerTable();
    }
    else if (input == 4) {
        // Drop the table
        if (dropPlayerTable() != 0) {
            std::cerr << "Failed to drop the table." << std::endl;
            return 1;
        }

        // Add test data after dropping the table
        if (addPlayerTestData() != 0) {
            std::cerr << "Failed to add test data." << std::endl;
            return 1;
        }
    }
    else {
        std::cerr << "Invalid input. Please select a valid option." << std::endl;
        return 1;
    }

    return 0;
}


//idea
//codeToRunINSQL("Write code here");