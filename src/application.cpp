#include "application.hpp"
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>
#include <iostream>

Application::Application() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error: " << SDL_GetError() <<std::endl;
        m_exit_status = 1;
    }

    m_window = std::make_unique<Window>(Window::Settings{"MTG Stock App" + std::string(" - ") + m_user.getUserName()});

    // TODO: Add sign in page

    m_loadFileBrowser.SetTypeFilters({".txt"});
    m_loadFileBrowser.SetTitle("Select a file");

    // Test
    m_user.loadCardsFromTxtFile("/home/ged1brg/Downloads/Deck - Izzet Phoenix.txt");

    
    m_user.createNewDeck("Deck - Izzet Phoenix", "Standard");
    m_user.getDeck("Deck - Izzet Phoenix").addNewCardsToDeck("Arclight Phoenix", 4);

    m_user.createNewDeck("UB Murktide", "Modern");
    m_user.getDeck("UB Murktide").addNewCardsToDeck("Murktide Regent", 4);
}

Application::~Application() {
    SDL_Quit();
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

    // Handle events
    while(m_running) 
    {
        // Handle event from user
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

        if(!m_minimized) {
            ImGui::DockSpaceOverViewport();
            
            // Menu bar
            if(ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("Collection")) {
                    if(ImGui::MenuItem("Add Collection from file...")) {
                        m_loadFileBrowser.Open();
                    }

                    if(ImGui::MenuItem("Save Collection to file...")) {
                        m_saveFileBrowser.Open();
                    }

                    ImGui::EndMenu();
                }
                
                if (ImGui::BeginMenu("View")) {
                    ImGui::MenuItem("Collection", nullptr, &m_show_collection);

                    ImGui::MenuItem("Decks", nullptr, &m_show_decks);

                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }
            
           
            // Handles which file to load and its functions
            fileExplorerHandler();
            
            // Render "some panel".
            if (m_show_collection) {
                ImGui::Begin("Collection", &m_show_collection);

                // Show table with the user collection
                displayUserCollection();
                
                if (ImGui::Button("Add Card"))
                    ImGui::OpenPopup("Add Card");

                addCardHandler();

                ImGui::End();
            }

            if(m_show_decks){
                ImGui::Begin("Decks", &m_show_decks);

                

                ImGui::Text("This is a test");
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

void Application::displayUserDeck(const std::string deck_name) 
{
    static ImGuiTableFlags flags =
                    ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;

    if(ImGui::BeginTable("table_advanced", 2, flags, ImVec2(0, ImGui::GetTextLineHeightWithSpacing() + 7 ), 0.0f)) 
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();

        for (const auto& card : m_user.getDeck(deck_name).getCards()) {
            ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.0F);
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(card.name.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted(std::to_string(card.value).c_str());
        }

        ImGui::EndTable();                 
    }
}

void Application::displayUserCollection() 
{
    static ImGuiTableFlags flags =
                    ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;

    if(ImGui::BeginTable("table_advanced", 3, flags, ImVec2(0, ImGui::GetTextLineHeightWithSpacing() + 7 ), 0.0f)) 
    {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Price",ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();

        for (const auto& card : m_user.getCollection()) {
            ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.0F);
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(card.name.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted(std::to_string(card.value).c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::TextUnformatted(std::to_string(1).c_str());
        }

        ImGui::EndTable();                 
    }
}

/// Handlers ///

void Application::addCardHandler() 
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
            m_user.addCard(input, 0);
            
            ImGui::CloseCurrentPopup();
        }

        if(ImGui::Button("Cancel")) {
            
            ImGui::CloseCurrentPopup();  
        }

        ImGui::EndPopup();
    }

}

void Application::fileExplorerHandler() {
    // display file dialog
    m_loadFileBrowser.Display();

    if(m_loadFileBrowser.HasSelected()) {
        
        dbg(m_loadFileBrowser.GetSelected().string());
        dbg(m_loadFileBrowser.GetCurrentDirectory().string());
        m_user.loadCardsFromTxtFile(m_loadFileBrowser.GetSelected().string());
        m_loadFileBrowser.ClearSelected();
    }

    // display file dialog
    m_saveFileBrowser.Display();

    if(m_saveFileBrowser.HasSelected()) {
        dbg(m_saveFileBrowser.GetSelected().string());
        m_user.saveCollectionToTxtFile(m_saveFileBrowser.GetSelected().string());
        m_saveFileBrowser.ClearSelected();
    }
}