#include "application.hpp"
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>
#include <iostream>

Application::Application() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error: " << SDL_GetError() <<std::endl;
        m_exit_status = 1;
    }

    m_window = std::make_unique<Window>(Window::Settings{"Hello, World!"});

    loadFileBrowser.SetTypeFilters({".txt"});
 
    user.loadCardsFromTxtFile("/home/ged1brg/Downloads/Deck - Izzet Phoenix.txt");
}

Application::~Application() {
    SDL_Quit();
}

int Application::run() {

    if(m_exit_status == 1) {
        ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        return m_exit_status;
    }

    //Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io{ImGui::GetIO()};
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplSDL2_InitForSDLRenderer(m_window->get_native_window(), m_window->get_native_renderer());

    ImGui_ImplSDLRenderer_Init(m_window->get_native_renderer());

    m_running = true;
    while(m_running) 
    {
        SDL_Event event{};
        while(SDL_PollEvent(&event) == 1) 
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if(event.type == SDL_QUIT) 
            {
                stop();
            }

            if (event.type == SDL_WINDOWEVENT &&
                event.window.windowID ==
                SDL_GetWindowID(m_window->get_native_window())
            ) {
                on_event(event.window);
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();
        
        loadFileBrowser.SetTitle("Select a file");

        if(!m_minimized) {
             // This is all that needs to be added for this:
            ImGui::DockSpaceOverViewport();

            // Menu bar
            if(ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("Collection")) {
                    if(ImGui::MenuItem("Add Collection from file...")) {
                        loadFileBrowser.Open();
                    }

                    if(ImGui::MenuItem("Save Collection to file...")) {
                        saveFileBrowser.Open();
                    }

                    ImGui::EndMenu();
                }
                
                if (ImGui::BeginMenu("View")) {
                    ImGui::MenuItem(
                        "Some Panel", nullptr, &m_show_some_panel
                    );
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }
            
            // Handles which file to load and its functions
            fileExplorerHandler();
            
            // Render "some panel".
            if (m_show_some_panel) {
                //TODO: Change to string in the future
                
                ImGui::Begin("Collection", &m_show_some_panel);
                ImGui::Text("User: %s", user.getUserName().c_str());
                ImGui::Separator();

                // Show table with the user collection
                displayUserCollection(user);
            
                if (ImGui::Button("Add Card"))
                    ImGui::OpenPopup("Add Card");

                popUpAddCard(user);

                ImGui::End();
            }
        }

        // Show demo window for inspiration
        ImGui::ShowDemoWindow(); 

        // Rendering
        ImGui::Render();

        SDL_SetRenderDrawColor(
            m_window->get_native_renderer(),
            // Gray clear color (rgba)
            100, 100, 100, 255
        );

        SDL_RenderClear(m_window->get_native_renderer());

        // Render data through the SDL renderer
        ImGui_ImplSDLRenderer_RenderDrawData(
            ImGui::GetDrawData()
        );

        SDL_RenderPresent(m_window->get_native_renderer());
    }

    return m_exit_status;
}

void Application::displayUserCollection(const User& user) 
{
    static ImGuiTableFlags flags =
                    ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;

    if(ImGui::BeginTable("table_advanced", 2, flags, ImVec2(0, ImGui::GetTextLineHeightWithSpacing() + 7 ), 0.0f)) 
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Price",ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        for (const auto& card : user.getCollection()) {
            ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.0F);
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(card.name.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted(std::to_string(card.value).c_str());
        }

        ImGui::EndTable();                 
    }
}

void Application::popUpAddCard(User& user) 
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Add Card", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        // State
        static char input[32]{ "" };

        // Code
        const bool is_input_text_enter_pressed = ImGui::InputText("##input", input, sizeof(input), ImGuiInputTextFlags_EnterReturnsTrue);
        const bool is_input_text_active = ImGui::IsItemActive();
        const bool is_input_text_activated = ImGui::IsItemActivated();

        if (is_input_text_activated)
            ImGui::OpenPopup("##popup");

        {
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
            //ImGui::SetNextWindowSize({ ImGui::GetItemRectSize().x, 0 });
            if (ImGui::BeginPopup("##popup", ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_ChildWindow))
            {
                //TODO: Have a list of cards to choose from a database
                static const char* options[] = {
                    "Teferi, Hero of Dominaria",
                    "Nicol Bolas, the Ravager",
                    "Nissa, Who Shakes the World",
                    "Chandra, Awakened Inferno"
                };

                for (int i = 0; i < IM_ARRAYSIZE(options); i++)
                {
                    if (strstr(options[i], input) == NULL)
                    continue;
                    if (ImGui::Selectable(options[i]))
                    {
                        strcpy(input, options[i]);
                    }
                }

                if (is_input_text_enter_pressed || (!is_input_text_active && !ImGui::IsWindowFocused()))
                    ImGui::CloseCurrentPopup();

                ImGui::EndPopup();
            }
        }
        
        if (ImGui::Button("Add Card")) {
            user.addCard(input, 0);
            
            ImGui::CloseCurrentPopup();
        }

        if(ImGui::Button("Cancel")) {
            
            ImGui::CloseCurrentPopup();  
        }

        ImGui::EndPopup();
    }

}

void Application::stop() 
{
    m_running = false;
}

void Application::on_event(const SDL_WindowEvent& event) {
    switch (event.event) {
    case SDL_WINDOWEVENT_CLOSE:
      return on_close();
    case SDL_WINDOWEVENT_MINIMIZED:
      return on_minimize();
    case SDL_WINDOWEVENT_SHOWN:
      return on_shown();
  }
}

void Application::on_minimize() {
  m_minimized = true;
}

void Application::on_shown() {
  m_minimized = false;
}

void Application::on_close() {
  stop();
}

void Application::fileExplorerHandler() {
    // display file dialog
    loadFileBrowser.Display();

    if(loadFileBrowser.HasSelected()) {
        
        dbg(loadFileBrowser.GetSelected().string());
        dbg(loadFileBrowser.GetCurrentDirectory().string());
        user.loadCardsFromTxtFile(loadFileBrowser.GetSelected().string());
        loadFileBrowser.ClearSelected();
    }

    // display file dialog
    saveFileBrowser.Display();

    if(saveFileBrowser.HasSelected()) {
        dbg(saveFileBrowser.GetSelected().string());
        user.saveCollectionToTxtFile(saveFileBrowser.GetSelected().string());
        saveFileBrowser.ClearSelected();
    }
}