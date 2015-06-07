#include "Logger.h"

unsigned Logger::s_currentLogLevel = WARNING;
std::mutex Logger::s_mutex;
