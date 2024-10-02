#include <memory>
#include <imgui.h>

#include <imgui-filebrowser/imfilebrowser.h>
#include "window.hpp"
#include <SDL.h>
#include "user.hpp"

#include "dbg.h"

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
        
        void displayUserCollection();

        void displayUserDeck(const std::string deck_name);

        void addCardHandler();
        void fileExplorerHandler();
            
    private:
        bool m_running{true};
        int m_exit_status;

        std::unique_ptr<Window> m_window{nullptr};

        User m_user{"Dani", "1234"};

        ImGui::FileBrowser m_saveFileBrowser{ImGuiFileBrowserFlags_EnterNewFilename};
        ImGui::FileBrowser m_loadFileBrowser;

        bool m_show_collection{true};
        bool m_show_decks{false};
        bool m_minimized{false};
};