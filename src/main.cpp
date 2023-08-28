#include "SQLiteDataProvider.hpp"
#include "ImguiApp.hpp"



void main()
{
    auto dataProvider = std::make_unique<SQLiteDataProvider>("./data.db");
    ImguiApp app(std::move(dataProvider));
    app.Run();
}
