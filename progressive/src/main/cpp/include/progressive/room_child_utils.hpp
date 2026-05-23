#pragma once
#include <string>

std::string parseChildRoomId(const std::string& json);
std::string getChildrenRooms(const std::string& json);
std::string buildChildEvent(const std::string& json);
