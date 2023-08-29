#pragma once

#include "DataModels.hpp"
#include <vector>

class IDataProvider
{
public:
    virtual ~IDataProvider() = default;
    virtual std::vector<TodoItem> GetTodoItems() const = 0 ;
    virtual void AddTodoItem(const TodoItem& item) = 0;
    virtual void CompleteTodoItem(std::uint64_t idemId) = 0;
    virtual void RemoveTodoItem(std::uint64_t idemId) = 0;
};

// q: how can we improve this interface?

// a: see below
// 1. add error handling
// 2. add async methods
// 3. add cancellation
// 4. add progress reporting
// 5. add paging
// 6. add sorting
// 7. add filtering
// 8. add search
// 9. add logging
// 10. add metrics
// 11. add tracing
// 12. add retry
// 13. add caching
// 14. add batching
// 15. add batching

// q: how can we implement point 1
// a: see below
// 1. add error handling
// 1.1. add error codes
// 1.2. add error messages
// 1.3. add error categories
// 1.4. add error severity
// 1.5. add error context
// 1.6. add error stack trace
// 1.7. add error source
// 1.8. add error time
    // 1.9. add error location
    // 1.10. add error thread id
    // 1.11. add error process id
    // 1.12. add error machine name
    // 1.13. add error user name
    // 1.14. add error domain
    // 1.15. add error host
    // 1.16. add error port
    // 1.17. add error protocol
    // 1.18. add error request
    // 1.19. add error response
    // 1.20. add error request time
    // 1.21. add error response time
    // 1.22. add error request size