#include "window.hpp"
#include <iostream>

Window::Window(const Settings& settings) {

    auto window_flags{
        static_cast<SDL_WindowFlags>(
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
            )
    };

    constexpr int window_center_flag{SDL_WINDOWPOS_CENTERED};

    m_window = SDL_CreateWindow(
        settings.title.c_str(),
        window_center_flag,
        window_center_flag,
        settings.width,
        settings.height,
        window_flags
    );

    auto renderer_flags{
        static_cast<SDL_RendererFlags>(
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
        )
    };
    m_renderer = SDL_CreateRenderer(
        m_window, -1, renderer_flags
    );

    if (m_renderer == nullptr) {
        std::cerr << "Error creating SDL_Renderer!" << std::endl;
        return;
    }

    // Set render scale for high DPI displays
    const float scale{get_scale()};
    SDL_RenderSetScale(m_renderer, scale, scale);
}

Window::~Window() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

SDL_Window* Window::get_native_window() const {
  return m_window;
}

SDL_Renderer* Window::get_native_renderer() const {
  return m_renderer;
}

float Window::get_scale() const  {
    int window_width{0};
    int window_height{0};
    SDL_GetWindowSize(
        m_window,
        &window_width, &window_height
    );

    int render_output_width{0};
    int render_output_height{0};
    SDL_GetRendererOutputSize(
        m_renderer,
        &render_output_width, &render_output_height
    );

    const auto scale_x{
        static_cast<float>(render_output_width) /
        static_cast<float>(window_width)
    };

    return scale_x;
}