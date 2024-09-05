#include "application.hpp"

#include <iostream>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>
#include <imgui.h>

Application::Application() {
 

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error: " << SDL_GetError() <<std::endl;
        m_exit_status = 1;
    }

    m_window = std::make_unique<Window>(Window::Settings{"Hello, World!"});
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

    // // Get proper display scaling for high DPI displays
    // const float font_scaling_factor{m_window->get_scale()};

    // // Font size will be 18pt
    // const float font_size{18.0F * font_scaling_factor};

    // // Load font and set as default with proper scaling
    // io.Fonts->AddFontFromFileTTF(
    //     "fonts/Manrope.ttf",
    //     font_size
    // );
    // io.FontDefault = io.Fonts->AddFontFromFileTTF(
    //     "fonts/Manrope.ttf",
    //     font_size
    // );
    // io.FontGlobalScale = 1.0F / font_scaling_factor;

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
        
        if(!m_minimized) {
             // This is all that needs to be added for this:
            ImGui::DockSpaceOverViewport();

            if(ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if(ImGui::MenuItem("Exit")) {
                        stop();
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
            
            // Render "some panel".
            if (m_show_some_panel) {
                ImGui::Begin("Some panel", &m_show_some_panel);
                ImGui::Text("Hello World");
                ImGui::End();
            }
        }

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