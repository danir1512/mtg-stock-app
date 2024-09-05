#include <string>
#include <SDL.h>

class Window {
    public:
        struct Settings {
            std::string title;
            const int width{1280};
            const int height{720};
        };

        explicit Window(const Settings& settings);

        ~Window();

        [[nodiscard]] SDL_Window* get_native_window() const;
        [[nodiscard]] SDL_Renderer* get_native_renderer() const;
        [[nodiscard]] float get_scale() const;

    private:
        SDL_Window* m_window{nullptr};
        SDL_Renderer* m_renderer{nullptr}; 
};