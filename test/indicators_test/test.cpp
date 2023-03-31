#include <iostream>
#include <thread>
#include <chrono>

#include "indicators/indeterminate_progress_bar.hpp"
#include "indicators/cursor_control.hpp"
#include "indicators/termcolor.hpp"

int main()
{

    indicators::IndeterminateProgressBar bar{
        indicators::option::BarWidth{40},
        indicators::option::Start{"["},
        indicators::option::Fill{"."},
        indicators::option::Lead{"<=>"},
        indicators::option::End{"]"},
        indicators::option::PostfixText{"Updates IBN-Net Policy"},
        indicators::option::ForegroundColor{indicators::Color::Yellow},
        indicators::option::FontStyles{
            std::vector<indicators::FontStyle>{indicators::FontStyle::Bold}}
    };

    indicators::ShowConsoleCursor(false);

    auto job = [&bar]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        bar.MarkAsCompleted();
        std::cout << termcolor::Bold << termcolor::Green
            << "System is up to data!\n" << termcolor::Reset;
    };
    std::thread job_completion_thread(job);

    while(!bar.IsCompleted()) {
        bar.Tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    job_completion_thread.join();

    indicators::ShowConsoleCursor(true);

    return 0;
}

