#include <SDL.h>


//fuente: https://lazyfoo.net/tutorials/SDL/23_advanced_timers/index.php
class Timer{
public:
	Timer();
	void start();
	void stop();
	void pause();
	void unpause();
	bool isStarted() const;
	bool isPaused() const;
	unsigned int getTicks() const;
private:
	unsigned int startTicks;
	unsigned int pausedTicks;
	bool paused;
	bool started;
};