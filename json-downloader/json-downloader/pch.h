// pch.h
#ifndef PCH_H
#define PCH_H

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <string>
#include <iomanip> 
#include <fstream>

#include <curl/curl.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#endif //PCH_H