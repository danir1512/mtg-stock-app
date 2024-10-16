#include "application.hpp"

extern "C" {
    char* fetch_web_page();
    void free_string(char* s);
}

int main()
{
    // Call the Rust function to fetch a web page
    char* result = fetch_web_page();

    std::cout << "Web page content: " << result << std::endl;

    // Free the memory allocated by Rust
    free_string(result);

    
    Application app{};
    return app.run();
}