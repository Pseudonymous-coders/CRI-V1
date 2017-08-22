#pragma once

#define DEBUG 1 //Comment/uncomment for debugging

#ifndef CRISERVER_CONFIG_H
#define CRISERVER_CONFIG_H

#include <iostream>
#include <algorithm>
#include <string>
#include <strings.h>
#include <map>
#include <dirent.h>
#include <fstream>
#include <streambuf>
#include <functional>
#include <cctype>
#include <locale>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include <pstreams/pstream.h>

#include "json/json.h"
#include "json/json-forwards.h"

/*
 * Criserver configuration file
 * Place all configs here (Precompiled)
 * Don't make them const just in case if it's
 * An argument within the actual runner
 */

unsigned int port = 3535;
const std::string pname = "--port="; //Arg name
const unsigned char plen = 7; //Arg name length
const std::string allchar = "*";

//General options
const std::string apps_location = "/usr/share/applications/";
const std::string backup_image_loc = "/usr/share/pixmaps/";
const std::string scan_dir[] = {"/usr/games", "/usr/local/games"}; //Games aren't added by default
const std::string end_app = ".desktop";
const std::string work_g = "/root/Downloads/.cri/";
const std::string work_i = "icons/";
const std::string ext_type = ".png";
const std::string run_pref = "xiwi ";//"xiwi ";
const std::string icondir = work_g + work_i;
const std::string gitbase = "raw.githubusercontent.com";
const std::string iconbase = "/Pseudonymous-coders/CRI/master/localSite/images/icons/"; //Get param for the image request

//Json options
const std::string appjson = "apps";
const std::string recentappjson = "recentapps";
const std::string recentname = "recentapps.json";

//Search app options
const std::string seperator_search = " - ";

//Version control and options
const std::string current_version = "0.1.1";
const std::string current_release = "Zooplankton";
const std::string current_date = "9/1/2016";

//Thread safety stuff also just memory holders
const unsigned char sleepWaitKill = 1; //Seconds to wait if kill happened
const unsigned char maxRecentApps = 20;

enum CODE_FIND
{
    OK_RET,
    GEN_ERROR,
    EXTENSION_ERROR,
    DISPLAY_ERROR
};

const std::string enumList[] =
{
    "OK_RET",
    "GEN_ERROR",
    "EXTENSION_ERROR",
    "DISPLAY_ERROR"

};

std::string errorName(int enumNum)
{
    return enumList[enumNum];
}

const std::string extErrFind = "unable to start display";
const std::string extFinish = "onnected to extension";

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;
typedef std::pair<int, std::vector<std::string>> simpleproc;
typedef std::pair<bool, CODE_FIND> codeproc;

#endif
