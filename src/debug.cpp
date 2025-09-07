#include <halcyon/debug.hpp>

#include "SDL3/SDL_error.h"

// https://stackoverflow.com/a/55875595
#ifdef HAL_WIN32_AUX_CONSOLE
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#endif

using namespace hal;

#ifdef HAL_WIN32_AUX_CONSOLE
namespace
{
    bool redirect_console_io()
    {
        bool  result { true };
        FILE* fp;

        // Redirect STDIN if the console has an input handle
        if (::GetStdHandle(STD_INPUT_HANDLE) != INVALID_HANDLE_VALUE)
        {
            if (::freopen_s(&fp, "CONIN$", "r", stdin) != 0)
                result = false;
            else
                std::setvbuf(stdin, NULL, _IONBF, 0);
        }

        // Redirect STDOUT if the console has an output handle
        if (::GetStdHandle(STD_OUTPUT_HANDLE) != INVALID_HANDLE_VALUE)
        {
            if (::freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
                result = false;
            else
                std::setvbuf(stdout, NULL, _IONBF, 0);
        }

        // Redirect STDERR if the console has an error handle
        if (::GetStdHandle(STD_ERROR_HANDLE) != INVALID_HANDLE_VALUE)
        {
            if (::freopen_s(&fp, "CONOUT$", "w", stderr) != 0)
                result = false;
            else
                std::setvbuf(stderr, NULL, _IONBF, 0);
        }

        // Make C++ standard streams point to console as well.
        std::ios_base::sync_with_stdio();

        // Clear the error state for each of the C++ standard streams.
        std::wcout.clear();
        std::cout.clear();
        std::wcerr.clear();
        std::cerr.clear();
        std::wcin.clear();
        std::cin.clear();

        return result;
    }

    bool release_console()
    {
        bool  result { true };
        FILE* fp;

        // Just to be safe, redirect standard IO to NUL before releasing.

        // Redirect STDIN to NUL
        if (::freopen_s(&fp, "NUL:", "r", stdin) != 0)
            result = false;
        else
            std::setvbuf(stdin, NULL, _IONBF, 0);

        // Redirect STDOUT to NUL
        if (::freopen_s(&fp, "NUL:", "w", stdout) != 0)
            result = false;
        else
            std::setvbuf(stdout, NULL, _IONBF, 0);

        // Redirect STDERR to NUL
        if (::freopen_s(&fp, "NUL:", "w", stderr) != 0)
            result = false;
        else
            std::setvbuf(stderr, NULL, _IONBF, 0);

        // Detach from console
        if (!::FreeConsole())
            result = false;

        return result;
    }

    void adjust_console_buffer(std::int16_t minLength)
    {
        // Set the screen buffer to be big enough to scroll some text
        CONSOLE_SCREEN_BUFFER_INFO conInfo;
        ::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);

        conInfo.dwSize.Y = std::max(conInfo.dwSize.Y, minLength);

        ::SetConsoleScreenBufferSize(::GetStdHandle(STD_OUTPUT_HANDLE), conInfo.dwSize);
    }

    bool create_new_console(std::int16_t minLength)
    {
        bool result { false };

        // Release any current console and redirect IO to NUL
        release_console();

        // Attempt to create new console
        if (::AllocConsole())
        {
            adjust_console_buffer(minLength);
            result = redirect_console_io();
        }

        return result;
    }

    bool AttachParentConsole(std::int16_t minLength)
    {
        bool result { false };

        // Release any current console and redirect IO to NUL
        release_console();

        // Attempt to attach to parent process's console
        if (::AttachConsole(ATTACH_PARENT_PROCESS))
        {
            adjust_console_buffer(minLength);
            result = redirect_console_io();
        }

        return result;
    }
}

debug::stream_opener::stream_opener(std::int16_t stream_size)
    : opened { create_new_console(stream_size) }
{
}

debug::stream_opener::~stream_opener()
{
    if (opened)
        release_console();

    // Not setting `opened` to `false` here because this
    // destructor won't run before the program ends.
}

const debug::stream_opener debug::m_streamGuard { 1024 };
#endif

c_string debug::last_error()
{
    if (const char* const err { ::SDL_GetError() }; err[0] == '\0')
        return "[no SDL error]";
    else
        return err;
}
