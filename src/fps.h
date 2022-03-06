#include <ctime>

class FPS {
public:
    clock_t current_ticks, delta_ticks;
    clock_t fps = 0;

    float deltaTime = 0.0f;

    std::vector<float> frameTimes;

    std::vector<std::string> events;

    float averageTime = 0.0f;

    void startUpdate() {
        current_ticks = clock();
    }

    int endUpdate() {
        delta_ticks = clock() - current_ticks;
        if (delta_ticks > 0)
            fps = CLOCKS_PER_SEC / delta_ticks;

        frameTimes.push_back(fps);
        if (frameTimes.size() > 400) {
            frameTimes.erase(frameTimes.begin());
        }

        for (float f : frameTimes) {
            averageTime += f;
        }
        averageTime /= frameTimes.size();

        if (fps < averageTime / 2.0f) {
            events.insert(events.begin(), "Frame Drop!");
        }

        if (fps == 0) return (int)fps;
        deltaTime =  1.0f / fps;

        return (int)fps;
    }
};

class Profiler {
public:
    clock_t current_ticks, delta_ticks;

    float deltaTime = 0.0f;

    std::vector<float> times;

    void startProfile() {
        current_ticks = clock();
    }

    float endProfile() {
        delta_ticks = clock() - current_ticks;

        times.push_back(delta_ticks);
        return delta_ticks;
    }
};