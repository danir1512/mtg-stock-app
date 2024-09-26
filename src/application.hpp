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
        void popUpAddCard(User& user);
        void displayUserCollection(const User& user);
        void fileExplorerHandler();
            
    private:
        bool m_running{true};
        int m_exit_status;

        std::unique_ptr<Window> m_window{nullptr};

        User user{"Dani", "1234"};

        ImGui::FileBrowser saveFileBrowser{ImGuiFileBrowserFlags_EnterNewFilename};
        ImGui::FileBrowser loadFileBrowser;

        bool m_show_some_panel{true};
        bool m_minimized{false};
};