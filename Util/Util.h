#pragma once
#include <string>
#include <vector>

std::string GetFormatedTime(uint64_t microSec=0);

std::string GetFormatedTime2();

void mSleep(const uint32_t &ms);

bool splitStr(std::vector<std::string>& dest, const std::string& str, const std::string &seprator);

void makeDaemon();