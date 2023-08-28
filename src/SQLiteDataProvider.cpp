#include "SQLiteDataProvider.hpp"
#include <filesystem>
#include <ctime>


namespace {
    int DummyCallback(void* data, int argc, char** argv, char** azColName)
    {
        // do nothing
        return 0;
    }

    int GetSchemeVersionCallback(void* data, int argc, char** argv, char** azColName)
    {
        auto version = *reinterpret_cast<int*>(data);
        version = std::stoi(argv[0]);
        return 0;
    }

    /// implement method which will add timestamp to item title. Example: "Buy milk" -> "Buy milk - 20.01.2023"
    // wrong implementation by copilot
    std::string AppendDataToTitle(const std::string& title)
    {
        const int MAXLEN = 80;
        char s[MAXLEN];
        time_t t = time(0);
        strftime(s, MAXLEN, "%d.%m.%Y", localtime(&t));
        return title + " - " + std::string(s);
    }

}

SQLiteDataProvider::SQLiteDataProvider(const std::string& dbPath)
    : m_dbPath(dbPath)
{
    auto initDb = !std::filesystem::exists(dbPath);

    // q: open database connection
    // a: see below
    sqlite3* db = nullptr;
    //auto rc = sqlite3_open(dbPath.c_str(), &db);
    // q: open database connection with read-write access and create if no db exists
    // a: see below
    auto rc = sqlite3_open_v2(dbPath.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(db));
    }
    m_db.reset(db);
    if (initDb)
    {
        rc = sqlite3_exec(m_db.get(), "CREATE TABLE Todos (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);", DummyCallback, nullptr, nullptr);
        if (rc != SQLITE_OK)
        {
            throw std::exception(sqlite3_errmsg(db));
        }
        sqlite3_stmt * schemeVersionStatement = nullptr;
        std::string pragmaVersion = "PRAGMA user_version = " + std::to_string(dbVersion) + std::string(";");
        rc = sqlite3_prepare_v2(m_db.get(), pragmaVersion.c_str(), -1, &schemeVersionStatement, nullptr);
        if (rc != SQLITE_OK)
        {
            throw std::exception(sqlite3_errmsg(db));
        }

        do
        {
            rc = sqlite3_step(schemeVersionStatement);
        } while (rc == SQLITE_BUSY);

        rc = sqlite3_finalize(schemeVersionStatement);
        if (rc != SQLITE_OK)
        {
            throw std::exception(sqlite3_errmsg(db));
        }
    }
    sqlite3_stmt *currentVersionStatement = nullptr;
    sqlite3_prepare_v2(db, "PRAGMA user_version;", -1, &currentVersionStatement, nullptr);
    sqlite3_step(currentVersionStatement);
    m_dbVersion = sqlite3_column_int(currentVersionStatement, 0);
    rc = sqlite3_finalize(currentVersionStatement);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(db));
    }
}

void SQLiteDataProvider::AddTodoItem(const TodoItem& item)
{

    // q: add item to database
    // a: see below
    sqlite3_stmt *addStatement = nullptr;
    auto rc = sqlite3_prepare_v2(m_db.get(), "INSERT INTO Todos (name) VALUES (?);", -1, &addStatement, nullptr);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    auto title = AppendDataToTitle(item.title);
    rc = sqlite3_bind_text(addStatement, 1, title.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    do
    {
        rc = sqlite3_step(addStatement);
    } while (rc == SQLITE_BUSY);

    rc = sqlite3_finalize(addStatement);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }
}



