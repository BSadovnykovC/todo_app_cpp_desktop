#pragma once

#include "IDataProvider.hpp"
#include <sqlite3.h>
#include <memory>

const int dbVersion = 2;
// q: implement IDataProvider using SQLite
// a: see below
class SQLiteDataProvider : public IDataProvider
{
private:
    // q: add deleter for sqlite3
    // a: see below
    struct SQLiteDeleter
    {
        void operator()(sqlite3* db) const
        {
            sqlite3_close_v2(db);
        }
    };

public:
    // q: add constructor with database path
    // a: see below
    SQLiteDataProvider(const std::string& dbPath);
    
    // q: add destructor
    // a: see below
    ~SQLiteDataProvider() final = default;

    std::vector<TodoItem> GetTodoItems() const final;
    void AddTodoItem(const TodoItem& item) final;

    // q: fix build error
    // a: see below
    void CompleteTodoItem(std::uint64_t idemId) final;
    void RemoveTodoItem(std::uint64_t idemId) final;
private:
    void InitDB();
    void CheckMigration();
    void HandleMigration();
    void FirstMigration();
private:
    // q: add private member for database path
    // a: see below
    std::string m_dbPath;
    // q: add private member for database connection
    // a: see below
    std::unique_ptr<sqlite3, SQLiteDeleter> m_db;
    uint64_t m_dbVersion = 0;
};