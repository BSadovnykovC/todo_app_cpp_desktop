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
        auto newTitle = title + " - " + std::string(s);
        return newTitle;
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
        InitDB();
    }

    sqlite3_stmt* currentVersionStatement = nullptr;
    rc = sqlite3_prepare_v2(m_db.get(), "PRAGMA user_version;", -1, &currentVersionStatement, nullptr);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }


    rc = sqlite3_step(currentVersionStatement);
    while (rc == SQLITE_ROW)
    {
        m_dbVersion = sqlite3_column_int(currentVersionStatement, 0);
        rc = sqlite3_step(currentVersionStatement);
    }

    if (rc != SQLITE_OK && rc != SQLITE_DONE)
    {
        throw std::exception(sqlite3_errmsg(db));
    }

    sqlite3_finalize(currentVersionStatement);
    CheckMigration();
}

void SQLiteDataProvider::AddTodoItem(const TodoItem& item)
{

    // q: add item to database
    // a: see below
    sqlite3_stmt* addStatement = nullptr;
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

    sqlite3_finalize(addStatement);
}

void SQLiteDataProvider::CompleteTodoItem(std::uint64_t idemId)
{
    sqlite3_stmt* completeStatement = nullptr;  
    auto rc = sqlite3_prepare_v2(m_db.get(), "UPDATE Todos SET completed = 1 WHERE id = (?);", -1, &completeStatement, nullptr);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    rc = sqlite3_bind_int64(completeStatement, 1, idemId);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    do
    {
        rc = sqlite3_step(completeStatement);
    } while (rc == SQLITE_BUSY);

    sqlite3_finalize(completeStatement);
}

void SQLiteDataProvider::RemoveTodoItem(std::uint64_t idemId)
{
    sqlite3_stmt* removeStatement = nullptr;
    auto rc = sqlite3_prepare_v2(m_db.get(), "DELETE FROM Todos WHERE id = (?);", -1, &removeStatement, nullptr);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    rc = sqlite3_bind_int64(removeStatement, 1, idemId);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    do
    {
        rc = sqlite3_step(removeStatement);
    } while (rc == SQLITE_BUSY);

    rc = sqlite3_finalize(removeStatement);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }
}


void SQLiteDataProvider::InitDB()
{
    auto rc = sqlite3_exec(m_db.get(), "CREATE TABLE Todos (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, completed INTEGER DEFAULT 0);", DummyCallback, nullptr, nullptr);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    std::string pragmaVersion = "PRAGMA user_version = " + std::to_string(dbVersion) + std::string(";");
    sqlite3_stmt* schemeVersionStatement = nullptr;
    rc = sqlite3_prepare_v2(m_db.get(), pragmaVersion.c_str(), -1, &schemeVersionStatement, nullptr);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    do
    {
        rc = sqlite3_step(schemeVersionStatement);
    } while (rc == SQLITE_BUSY);

    rc = sqlite3_finalize(schemeVersionStatement);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }
}

void SQLiteDataProvider::CheckMigration()
{
    if (m_dbVersion != dbVersion)
    {
        HandleMigration();
    }
}

void SQLiteDataProvider::HandleMigration()
{
    switch (dbVersion - m_dbVersion)
    {
    case 1:
        FirstMigration();
        break;
    default:
        break;
    }
}

void SQLiteDataProvider::FirstMigration()
{
    // q: migrate database
     // a: see below
    sqlite3_stmt* migrateStatement = nullptr;
    auto rc = sqlite3_prepare_v2(m_db.get(), "ALTER TABLE Todos ADD COLUMN completed INTEGER DEFAULT 0;", -1, &migrateStatement, nullptr);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    do
    {
        rc = sqlite3_step(migrateStatement);
    } while (rc == SQLITE_BUSY);

    rc = sqlite3_finalize(migrateStatement);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    std::string pragmaVersion = "PRAGMA user_version = " + std::to_string(dbVersion) + std::string(";");
    sqlite3_stmt* schemeVersionStatement = nullptr;
    rc = sqlite3_prepare_v2(m_db.get(), pragmaVersion.c_str(), -1, &schemeVersionStatement, nullptr);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    do
    {
        rc = sqlite3_step(schemeVersionStatement);
    } while (rc == SQLITE_BUSY);

    rc = sqlite3_finalize(schemeVersionStatement);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }
}


// implement GetTodoItems to get all items from database Todos table
std::vector<TodoItem> SQLiteDataProvider::GetTodoItems() const
{
    std::vector<TodoItem> items;
    // q: get all items from database
    // a: see below
    sqlite3_stmt* getStatement = nullptr;
    auto rc = sqlite3_prepare_v2(m_db.get(), "SELECT name, id, completed FROM Todos;", -1, &getStatement, nullptr);
    if (rc != SQLITE_OK)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    rc = sqlite3_step(getStatement);

    while (rc == SQLITE_ROW)
    {
        TodoItem item;
        item.title = reinterpret_cast<const char*>(sqlite3_column_text(getStatement, 0));
        item.id = sqlite3_column_int64(getStatement, 1);
        item.completed = sqlite3_column_int64(getStatement, 2);
        items.push_back(item);
        rc = sqlite3_step(getStatement);
    }

    if (rc != SQLITE_OK && rc != SQLITE_DONE)
    {
        throw std::exception(sqlite3_errmsg(m_db.get()));
    }

    sqlite3_finalize(getStatement);
    return items;
}


