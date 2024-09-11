#include <memory>
#include "window.hpp"
#include <SDL.h>
#include "user.hpp"

class Application {
    public:
        Application();
        ~Application();

        int run();
        void stop();   
        void on_event(const SDL_WindowEvent& event);

        // Specialised events
        void on_minimize();
        void on_shown();
        void on_close();
        void popUpAddCard(const User& user);
        void displayUserCollection(const User& user);
            
    private:
        bool m_running{true};
        int m_exit_status;

        std::unique_ptr<Window> m_window{nullptr};

        bool m_show_some_panel{true};
        bool m_minimized{false};
};