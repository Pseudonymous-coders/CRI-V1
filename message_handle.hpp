#include "config.h" //Just in case if it's not imported

#ifndef CRISERVER_MESSAGEHANDLE_HPP
#define CRISERVER_MESSAGEHANDLE_HPP

#include <boost/regex.hpp>
#include <sys/stat.h>
#include <utility>

using namespace redi;

static std::string blank = "";
static std::string search_all = "*";
static pstreams::pmode modeout = pstreams::pstdout|pstreams::pstderr;

int lastwantedrecent = maxRecentApps; //Recent apps load

CODE_FIND coderet = OK_RET;
codeproc retType;

struct app_build
{
    std::string name;
    std::string comment = "N/A";
    std::string icon_path;
    std::string executable;
};

struct search_build
{
    std::string name;
    std::string desc;
};

std::vector<app_build> loaded_apps;
std::deque<app_build> recent_apps;

#include "jsoner.hpp" //We need app build for the proc message

class system
{
public:
    static simpleproc simple(const std::string &torun)
    {
        std::vector<std::string> combined;
        int exitcode = 32512;
        try
        {
            ipstream child(torun.c_str(), modeout);
            std::string mess, errmess;
            while(std::getline(child, mess) || std::getline(child, errmess))
            {
                combined.push_back(mess);
                if(errmess.length() > 0) combined.push_back("ERR: " + errmess);
            }
            child.close();
            exitcode = child.rdbuf()->status();
        }
        catch(...)
        {
            std::cerr << "ERROR in child process or initialize of process" << std::endl;
        }
        return simpleproc (exitcode, combined);
    }

    //Simple method to write to a file
    static inline void write_file(const std::string &path, const std::string &towrite)
    {
        try
        {
            std::ofstream tow(path);
            tow << towrite;
            tow.flush();
            tow.close();
        }
        catch(std::exception & err)
        {
            std::cerr << "Error writing to file: " << err.what() << std::endl;
        }
    }

    //Simple method to read from file
    static inline std::string read_file(const std::string &path)
    {
        std::string toret;
        try
        {
            std::ifstream tor(path);
            toret = std::string((std::istreambuf_iterator<char>(tor)), std::istreambuf_iterator<char>());
            tor.close();
        }
        catch(std::exception & err)
        {
            std::cerr << "Reading file error: " << err.what() << std::endl;
        }
        return toret;
    }

    static inline std::string get_path()
    {
        try
        {
            char* path;
            path = getenv("PATH");
            if(path == NULL)
            {
                std::cerr << "Failed retrieving path!!!" << std::endl;
                return "";
            }
#ifdef DEBUG
            std::cout << "DEBUG: Got path: " << path << std::endl;
#endif // DEBUG
            return std::string(path);
        }
        catch(std::exception &err)
        {
            return "";
        }
    }

    static inline bool set_path()
    {
        std::string curpath = system::get_path();
        if(curpath.length() == 0) return false; //Error has occured do not continue
        try
        {
            for(std::string in : scan_dir)
            {
                if(curpath.find(in) != std::string::npos)
                {
#ifdef DEBUG
                    std::cout << "DEBUG: " << in << " already in path!" << std::endl;
#endif // DEBUG
                    continue; //Don't add if it already exists
                }
                if(curpath.at(curpath.size() - 1) == ':') curpath += in + ":";
                else curpath += ":" + in;
                std::cout << "ADDING: " << in << std::endl;
            }
            std::string setPath = "PATH=" + curpath;
            char *pPath = new char[setPath.length() + 1]; //Add EOF character
            strcpy(pPath, setPath.c_str());
#ifdef DEBUG
            std::cout << "DEBUG: Setting new path: " << curpath << std::endl;
#endif // DEBUG
            putenv(pPath);
            //Do not delete the char array it's dynamically allocated and needs to stay that way
            //delete [] pPath;
            //simpleproc ret = system::simple(setPath);
            //if(ret.first != 0) return false;

            simpleproc ret = system::simple("echo $PATH");
            std::string ased = "";
            for (const std::string &s : ret.second)
            {
                ased += s;
            }

#ifdef DEBUG
            std::cout << "DEBUG: Double check path: " << ased << std::endl;
#endif // DEBUG
            if(ased.find(curpath) == std::string::npos)
            {
#ifdef DEBUG
                std::cout << "DEBUG: Setting path failed!" << std::endl;
#endif // DEBUG
                return false;
            }
            return true;
        }
        catch(std::exception &err)
        {
            std::cerr << "Failed setting path!" << std::endl;
        }
        return true;
    }
};

class procmessage
{
public:
    static void send_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg, std::stringstream *tosend)
    {
        try
        {
            s->send(hdl, (*tosend).str().c_str(), msg->get_opcode());
        }
        catch(const websocketpp::lib::error_code& error)
        {
            std::cerr << "Send failed: " << std::endl;
        }
        catch(...)
        {
            std::cerr << "Send fatal failed" << std::endl;
        }
    }

    static inline void lower(std::string *current)
    {
        std::transform((*current).begin(), (*current).end(), (*current).begin(), ::tolower);
    }

    static inline bool equals(const std::string &first, const std::string &second)
    {
        return (bool) !(first.compare(second));
    }

    static inline bool contains(const std::string &full, const std::string &contains)
    {
        return (full.find(contains) != std::string::npos);
    }

    static bool replace(std::string &start, const std::string &from, const std::string &to)
    {
        size_t start_pos = start.find(from);
        if(start_pos == std::string::npos)
            return false;
        start.replace(start_pos, from.length(), to);
        return true;
    }

    //Trim code from SO: http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    // trim from start (in place)
    static inline void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                        std::not1(std::ptr_fun<int, int>(std::isspace))));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                             std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string &s)
    {
        ltrim(s);
        rtrim(s);
    }

    static inline bool filetest(const std::string &path)
    {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }

    static std::vector<std::string> getDir(const std::string &folder, const std::string *contain = &search_all)
    {
        std::vector<std::string> files;
        DIR *dir;
        struct dirent *ent;
        bool searches = equals((*contain), allchar);
        if ((dir = opendir(folder.c_str())) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                std::string curname = ent->d_name;
                if(searches)
                {
                    files.push_back(folder + curname);
#ifdef DEBUG
                    std::cout << "DEBUG: ADDING: " << curname << std::endl;
#endif // DEBUG
                }
                else
                {
                    if(contains(curname, (*contain)))
                    {
                        files.push_back(folder + curname);
#ifdef DEBUG
                        std::cout << "DEBUG: ADDING: " << curname << std::endl;
#endif // DEBUG
                    }
#ifdef DEBUG
                    else
                    {
                        std::cout << "DEBUG: NOT ADDING: " << curname << std::endl;
                    }
#endif // DEBUG
                }
            }
            closedir(dir);
        }
        return files;
    }

    static std::string get_prop(const std::string &file, const std::string &prop)
    {
        std::string tofind = prop + "=(.*?)\n";
        std::string result;
        boost::smatch what;
        //boost::match_results<std::string::const_iterator> results;
        boost::regex get_propr(tofind, boost::regex::icase); //Ignore specific case
        if(boost::regex_search(file, what, get_propr))
        {
            result = std::string(what[1].first, what[1].second);
#ifdef DEBUG
            std::cout << "DEBUG: PROP(" << prop << "): " << result << std::endl;
#endif // DEBUG
        }
        trim(result); //Remove not needed whitespace
        return result;
    }

    static void get_app_back(std::string &curfile, std::vector<app_build> &applications, std::vector<std::string> desktopFiles, std::string &response)
    {
        //@TODO add multithreaded support
        //boost::mutex mutex;
        std::string filecontent;
        std::string name;
        std::string executable;

        {
            //boost::mutex::scoped_lock s(mutex);
            filecontent = system::read_file(curfile);
            name = get_prop(filecontent, "name"); //Name
            executable = get_prop(filecontent, "exec"); //Executable path
        }

        if(name.length() == 0 || executable.length() == 0) return;

        //Remove the .desktop executable features that start with an %
        if(procmessage::contains(executable, "%"))
        {
            boost::regex perc(" %([a-zA-Z])"); //Example option is ( %U)
            std::string fmt("(?1)");
            executable = boost::regex_replace(executable, perc, fmt, boost::match_default | boost::format_all);
        }

        app_build addapp;
        addapp.name = name;
        addapp.executable = executable;
        std::string comment = get_prop(filecontent, "comment"); //Comment

        if(comment.length() > 0) addapp.comment = comment;

        std::string icon = get_prop(filecontent, "icon"); //Icon name/path
        if(icon.length() > 0)
        {
            size_t n = std::count(icon.begin(), icon.end(), '/'); //To see if it's an absolute path
            std::string fullpath = icon;
            if(n > 2)   //If it's an absolute path
            {
                //@TODO load absolute paths into the git reg
                fullpath = fullpath.substr(fullpath.rfind("/")); //If it is a full path just make a temp for now
                //std::string newname = base.substr(0, base.rfind(".")) + ext_type;
                //CImg<unsigned char> loaded(fullpath);
                //loaded.save(newname);
            }
            if(procmessage::contains(fullpath, "."))
            {
                boost::regex dotter("~.([^.]*)$~"); //Example option is ( ok.png)
                std::string fmt("(?1)");
                fullpath = boost::regex_replace(fullpath, dotter, fmt, boost::match_default | boost::format_all);
                //fullpath = fullpath.substr(0, fullpath.rfind(".")); //Remove the extension type such as .svg
            }
            addapp.icon_path = fullpath; //Set current build icon
        }
        {
            //boost::mutex::scoped_lock s(mutex);
            applications.push_back(addapp);
        }
        //if(contains(icon, "/"))
    }

    static std::string get_recent_apps(unsigned int wantedapps)
    {
        boost::mutex mutex;
        {
            boost::mutex::scoped_lock s(mutex);
            lastwantedrecent = wantedapps;
        }
        std::string response = jsoner::Error("recentapps_error", 6);
        std::vector<app_build> recapps;
        {
            boost::mutex::scoped_lock s(mutex);
            if(recent_apps.size() > maxRecentApps) recent_apps.erase(recent_apps.begin() + maxRecentApps, recent_apps.end()); //Keep recent apps small and not eat up memory
            recapps = std::vector<app_build>(recent_apps.begin(), recent_apps.end());
        }

        if(recapps.size() > wantedapps) recapps.erase(recapps.begin() + wantedapps, recapps.end()); //Only compile
        //The apps that the user has requested

        if(recapps.size() > 0)
        {
            response = jsoner::ApplicationsUpdate(recentappjson, recapps);
        }
        else response = "{\"" + recentappjson + "\":[],\"num\":0}";
        return response;
    }

    static std::string get_apps()
    {
        std::string response = jsoner::Error("generalerror", 0);
        std::vector<std::string> desktopFiles = getDir(apps_location, &end_app);
        std::vector<app_build> applications;
        //boost::thread_group worker_threads;

        for(std::vector<std::string>::iterator it = desktopFiles.begin(); it != desktopFiles.end(); it++)
        {
            std::string curfile = *it;
#ifdef DEBUG
            std::cout << "DEBUG: PROOCESSING: " << curfile << std::endl;
#endif // DEBUG

            //@TODO add multithreaded getApps
            procmessage::get_app_back(curfile, applications, desktopFiles, response);

            //boost::thread new_t(boost::bind(&procmessage::get_app_back, curfile, applications, desktopFiles, response));

            //worker_threads.add_thread(&new_t);
            //while(!worker_threads.is_thread_in(&new_t));
        }
        //worker_threads.join_all(); //Wait for all threads to finish
        //sleepmicro(25);
        //worker_threads.join_all();
        //size_t num_t = worker_threads.size();
        //if(num_t > 0)
        if(applications.size() > 0)
        {
            response = jsoner::ApplicationsUpdate(appjson, applications);
            loaded_apps = applications; //Update global apps settings
        }
        else response = "{\"" + appjson + "\":[],\"num\":0}";
        return response;
    }

    static void back_graph(const std::string &run, const std::string &namerun, server *s, websocketpp::connection_hdl hdl, message_ptr msg)
    {
        coderet = OK_RET;

        try
        {
            std::stringstream streamstart = jsoner::StartAppRun(namerun);
            std::stringstream updatestream = jsoner::UpdateAppRun(namerun);
            std::stringstream errorstream;
            send_message(s, hdl, msg, &streamstart);
            int exitcode = 1;
            try
            {
                ipstream child(run.c_str(), modeout);
                std::string mess, errmess;
                while(std::getline(child, mess) || std::getline(child, errmess))
                {
                    std::string line = mess + errmess;
#ifdef DEBUG
                    std::cout << "DEBUG: LINE OF CHILD: " << line << std::endl;
#endif // DEBUG
                    lower(&line);
                    if(contains(line, extErrFind))
                    {
                        coderet = EXTENSION_ERROR;
#ifdef DEBUG
                        std::cerr << "DEBUG: Error found extension error: " << line << std::endl;
#endif // DEBUG
                        child.close();
                        exitcode = child.rdbuf()->status();
                        errorstream << jsoner::ErrorApp(namerun, "extension failed loading");
                        send_message(s, hdl, msg, &errorstream);
                        return;
                    }
                    else if(contains(line, extFinish))
                    {
                        coderet = OK_RET;
#ifdef DEBUG
                        std::cout << "DEBUG: Application connected to extension" << std::endl;
#endif // DEBUG
                        std::stringstream finished = jsoner::ConnectedAppRun(namerun);
                        send_message(s, hdl, msg, &finished);
                        return;
                    }
                    else
                    {
#ifdef DEBUG
                        std::cout << "DEBUG: SENDING UPDATE" << std::endl;
#endif // DEBUG
                        send_message(s, hdl, msg, &updatestream);
                    }
                }
                child.close();
                exitcode = child.rdbuf()->status();
                std::stringstream donestream = jsoner::EndAppRun(namerun, (exitcode == 0));
                send_message(s, hdl, msg, &donestream);
                if(exitcode != 0)
                {
                    std::stringstream nonzero;
                    nonzero << jsoner::ErrorApp(namerun, "Exit with non 0 status code! ");
                    send_message(s, hdl, msg, &nonzero);
                }
            }
            catch(...)
            {
                std::cerr << "ERROR in child process or initialize of process" << std::endl;
                std::stringstream errors;
                errors << jsoner::ErrorApp(namerun, "App failed pstream initialization");
                send_message(s, hdl, msg, &errors);
            }
#ifdef DEBUG
            std::cout << "DEBUG: Child process exit: " << exitcode << std::endl;
#endif // DEBUG
        }
        catch(std::exception& err)
        {
            std::cerr << "Caught error: " << err.what() << std::endl;
        }
    }

    static void save_recent_back()
    {
        boost::mutex mutex;
        std::deque<app_build> r_apps;
        std::string appswrite = get_recent_apps(lastwantedrecent); //Save recent apps from last wanted to load them
        {
            boost::mutex::scoped_lock s(mutex);
            r_apps = recent_apps;
        }

        std::vector<app_build> recapps(r_apps.begin(), r_apps.end());
        std::string response;
        if(recapps.size() > 0)
        {
            response = jsoner::ApplicationsUpdate(recentappjson, recapps);
        }
        else response = "{\"" + recentappjson + "\":[],\"num\":0}";

        system::write_file(recentname, response);
    }

    static void sleepfor(int seconds)
    {
        boost::this_thread::sleep_for(boost::chrono::seconds(seconds));
    }

    static void sleepmicro(unsigned int micros)
    {
        boost::this_thread::sleep_for(boost::chrono::microseconds(micros));
    }

    static void run_save()
    {
        try
        {
            boost::thread t(boost::bind(procmessage::save_recent_back));
        }
        catch(boost::exception &err)
        {
            std::cerr << "Error saving in thread attaching to default" << std::endl;
            procmessage::save_recent_back();
        }
        catch(...)
        {
            std::cerr << "General system error in save thread" << std::endl;
        }
    }

    //Load the local settings back into program
    static void run_load()
    {
        boost::mutex mutex;

        std::deque<app_build> toload_apps;
        Json::Value root;
        Json::Reader reader;

        std::stringstream toparse;
        toparse << system::read_file(recentname);
        if(!reader.parse(toparse, root))
        {
#ifdef DEBUG
            std::cerr << "DEBUG: Error reading recent apps" << std::endl;
#endif // DEBUG
            return;
        }

        if(!root.isMember(recentappjson))
        {
#ifdef DEBUG
            std::cerr << "DEBUG: Error recent apps json incomplete" << std::endl;
#endif // DEBUG
            return;
        }

        const Json::Value appsJson = root[recentappjson];
        for(unsigned int ind = 0; ind < appsJson.size(); ind++)
        {
            Json::Value appjson = appsJson[ind];
            app_build tobuild;

            tobuild.name = appjson.get("name", "Error").asString();
            tobuild.comment = appjson.get("desc", "N/A").asString();
            tobuild.icon_path = appjson.get("icon", "").asString();
            tobuild.executable = appjson.get("exec", "").asString();

            toload_apps.push_back(tobuild);
        }
        recent_apps = toload_apps; //When properly loaded set new apps
    }

    static void run_search(const std::string &search_req, server *s, websocketpp::connection_hdl hdl, message_ptr msg)
    {
        simpleproc getres = system::simple("apt-cache search " + search_req); //Search via apt-cache
        std::string response;

        if(getres.first != 0)
        {
            std::stringstream errorsend;
            errorsend << jsoner::Error("search", getres.first);
            procmessage::send_message(s, hdl, msg, &errorsend);
        }

        std::vector<std::string> apps = getres.second;
        std::vector<search_build> t_apps_load;

        for(std::string curapp : apps)
        {
#ifdef DEBUG
            std::cout << "DEBUG: Found app in search: " << curapp << std::endl;
#endif // DEBUG
            search_build t_app;
            if(!procmessage::contains(curapp, seperator_search))
            {
#ifdef DEBUG
                std::cout << "DEBUG: Not a valid app: " << curapp << std::endl;
#endif // DEBUG
                continue;
            }
            size_t sep_pos = curapp.find(seperator_search);
            t_app.name = curapp.substr(0, sep_pos);
            t_app.desc = curapp.substr(sep_pos + seperator_search.length());
#ifdef DEBUG
            std::cout << "DEBUG: Search added (NAME): " << t_app.name << " (DESC):" << t_app.desc << std::endl;
#endif // DEBUG
            t_apps_load.push_back(t_app);
        }

        if(t_apps_load.size() > 0)
        {
            response = jsoner::SearchUpdate(t_apps_load);
        }
        else response = "{\"search\":[],\"num\":0}";
        std::stringstream tosend;
        tosend << response;
        procmessage::send_message(s, hdl, msg, &tosend);
    }

    static void message_mutex(server *s, websocketpp::connection_hdl hdl, message_ptr msg, std::stringstream *send, boost::mutex &mutex)
    {
        {
            boost::mutex::scoped_lock sl(mutex);
            procmessage::send_message(s, hdl, msg, send);
        }
    }

    static void run_install_back(const std::string &run, server *s, websocketpp::connection_hdl hdl, message_ptr msg)
    {
        //simpleproc ret = system::simple("printf 'Y\ny\n' | apt-get install " + run);
        boost::mutex mutex;

        std::stringstream starter_s = jsoner::StartAppInstall(run);
        std::stringstream errorer_s = jsoner::ErrorAppInstall(run);
        std::stringstream donerer_s = jsoner::DoneAppInstall(run);
        message_mutex(s, hdl, msg, &starter_s, mutex);
        std::string runner = "printf 'Y\ny\n' | apt-get --force-yes install " + run;
        int exitcode = 1;
        try
        {
            ipstream child(runner.c_str(), modeout);
            std::string mess, errmess;
            while(std::getline(child, mess) || std::getline(child, errmess))
            {
                std::string line = mess + errmess;
#ifdef DEBUG
                std::cout << "DEBUG: LINE OF INSTALL CHILD: " << line << std::endl;
#endif // DEBUG
                std::stringstream updater_s = jsoner::UpdateAppInstall(run, line);
                message_mutex(s, hdl, msg, &updater_s, mutex);
            }
            child.close();
            exitcode = child.rdbuf()->status();
            send_message(s, hdl, msg, &donerer_s);
            if(exitcode != 0)
            {
                message_mutex(s, hdl, msg, &errorer_s, mutex);
            }
        }
        catch(...)
        {
            std::cerr << "ERROR in child process or initialize of process" << std::endl;
            std::stringstream errors;
            errors << jsoner::Error("installerror general", 13);
            send_message(s, hdl, msg, &errors);
        }
#ifdef DEBUG
        std::cout << "DEBUG: Child process exit: " << exitcode << std::endl;
#endif // DEBUG
    }

    static void run_remove_back(const std::string &run, server *s, websocketpp::connection_hdl hdl, message_ptr msg)
    {
        //simpleproc ret = system::simple("printf 'Y\ny\n' | apt-get install " + run);
        boost::mutex mutex;

        std::stringstream starter_s = jsoner::StartAppRemove(run);
        std::stringstream errorer_s = jsoner::ErrorAppRemove(run);
        std::stringstream donerer_s = jsoner::DoneAppRemove(run);
        message_mutex(s, hdl, msg, &starter_s, mutex);
        std::string runner = "printf 'Y\ny\n' | apt-get --force-yes remove " + run;
        int exitcode = 1;
        try
        {
            ipstream child(runner.c_str(), modeout);
            std::string mess, errmess;
            while(std::getline(child, mess) || std::getline(child, errmess))
            {
                std::string line = mess + errmess;
#ifdef DEBUG
                std::cout << "DEBUG: LINE OF REMOVE CHILD: " << line << std::endl;
#endif // DEBUG
                std::stringstream updater_s = jsoner::UpdateAppRemove(run, line);
                //Send update response
                message_mutex(s, hdl, msg, &updater_s, mutex);
            }
            child.close();
            exitcode = child.rdbuf()->status();
            send_message(s, hdl, msg, &donerer_s);
            if(exitcode != 0)
            {
                message_mutex(s, hdl, msg, &errorer_s, mutex);
            }
        }
        catch(...)
        {
            std::cerr << "ERROR in child process or initialize of process" << std::endl;
            std::stringstream errors;
            errors << jsoner::Error("removeerror general", 13);
            send_message(s, hdl, msg, &errors);
        }
#ifdef DEBUG
        std::cout << "DEBUG: Child process exit: " << exitcode << std::endl;
#endif // DEBUG
    }

    static std::stringstream run_stop(const std::string &tostop)
    {
        std::stringstream response_json;
        std::string prerun = tostop;
        size_t app_size = loaded_apps.size();
        if(app_size == 0) procmessage::get_apps(); //Run get apps to update list if it's empty
        if(app_size > 0)
        {
            for(std::vector<app_build>::iterator it = loaded_apps.begin(); it != loaded_apps.end(); it++)
            {
                app_build cur = *it;
                if(procmessage::equals(cur.name, prerun))   //If it's an already loaded name get the executable path
                {
                    prerun = cur.executable; //Set new executable
#ifdef DEBUG
                    std::cout << "DEBUG: Found executable by loaded name" << std::endl;
#endif // DEBUG
                    break;
                }
            }
        }

        std::string runstop_s = "pkill " + prerun;
        std::string runstop_s_2 = "killall " + prerun;
        simpleproc proc_ret = system::simple(runstop_s);
        simpleproc proc_ret_2 = system::simple(runstop_s_2);
        if(proc_ret.first != 0 && proc_ret_2.first != 0) {
            std::string compile = "";
            for(std::string inerr : proc_ret.second) compile += inerr;
            response_json = jsoner::ErrorAppKill(tostop, compile);
            return response_json;
        }
        response_json = jsoner::EndAppKill(tostop);
        return response_json;
    }

    static void run_update_back(server *s, websocketpp::connection_hdl hdl, message_ptr msg) {
        //simpleproc ret = system::simple("printf 'Y\ny\n' | apt-get install " + run);
        boost::mutex mutex;
        std::string run = "";

        std::stringstream starter_s = jsoner::StartAppRemove(run);
        std::stringstream errorer_s = jsoner::ErrorAppRemove(run);
        std::stringstream donerer_s = jsoner::DoneAppRemove(run);
        message_mutex(s, hdl, msg, &starter_s, mutex);
        std::string runner = "printf 'Y\ny\n' | apt-get --force-yes update;printf 'Y\ny\n' | apt-get --force-yes upgrade";
        int exitcode = 1;
        try
        {
            ipstream child(runner.c_str(), modeout);
            std::string mess, errmess;
            while(std::getline(child, mess) || std::getline(child, errmess))
            {
                std::string line = mess + errmess;
#ifdef DEBUG
                std::cout << "DEBUG: LINE OF REMOVE CHILD: " << line << std::endl;
#endif // DEBUG
                std::stringstream updater_s = jsoner::UpdateAppRemove(run, line);
                //Send update response
                message_mutex(s, hdl, msg, &updater_s, mutex);
            }
            child.close();
            exitcode = child.rdbuf()->status();
            send_message(s, hdl, msg, &donerer_s);
            if(exitcode != 0)
            {
                message_mutex(s, hdl, msg, &errorer_s, mutex);
            }
        }
        catch(...)
        {
            std::cerr << "ERROR in child process or initialize of process" << std::endl;
            std::stringstream errors;
            errors << jsoner::Error("updateerror general", 13);
            send_message(s, hdl, msg, &errors);
        }
#ifdef DEBUG
        std::cout << "DEBUG: Child process exit: " << exitcode << std::endl;
#endif // DEBUG
    }

    static void run_install(const std::string &to_install, server *s, websocketpp::connection_hdl hdl, message_ptr msg)
    {
        boost::thread t(boost::bind(procmessage::run_install_back, to_install, s, hdl, msg));
    }

    static void run_remove(const std::string &to_remove, server *s, websocketpp::connection_hdl hdl, message_ptr msg)
    {
        boost::thread t(boost::bind(procmessage::run_remove_back, to_remove, s, hdl, msg));
    }

    static void run_graphical(const std::string &run, const std::string &namerun, server *s, websocketpp::connection_hdl hdl, message_ptr msg)
    {
        boost::thread t(boost::bind(procmessage::back_graph, run, namerun, s, hdl, msg));
    }
};

class messagehandler
{
public:
    static void on_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg)
    {
        std::string message(msg->get_payload());
#ifdef DEBUG
        std::cout << "DEBUG MESSAGE: " << message << std::endl << "HDL: " << hdl.lock().get() << std::endl;
#endif
        Json::Value root;
        Json::Reader reader;

        if(!reader.parse(message, root, false))
        {
#ifdef DEBUG
            std::cerr << "DEBUG: Json parsing failed!" << std::endl;
#endif // DEBUG
            std::stringstream tos;
            tos << jsoner::Error("Json parse error", 1);
            procmessage::send_message(s, hdl, msg, &tos);
            return;
        }

        if(!root.isMember("exec"))
        {
            std::stringstream tos;
            tos << jsoner::Error("Json doesn't have exec", 1);
            procmessage::send_message(s, hdl, msg, &tos);
            return;
        }

        const Json::Value executable = root["exec"];

        std::string toexec = executable.asString();
        if(procmessage::equals(toexec, "stopserver"))
        {
            s->stop_listening();
            return;
        }
        else if(procmessage::equals(toexec, "run"))
        {
#ifdef DEBUG
            std::cout << "DEBUG: Running app " << std::endl;
#endif
            if(!root.isMember("app"))
            {
                std::stringstream tos;
                tos << jsoner::Error("Json parse error, app key failed", 1);
                procmessage::send_message(s, hdl, msg, &tos);
                return;
            }

            //Check already existing applications for an executable
            std::string prerun = root["app"].asString();
            std::string namemap = prerun;
            bool found_desktop_app = false;
            size_t app_size = loaded_apps.size();
            if(app_size == 0) procmessage::get_apps(); //Run get apps to update list if it's empty
            if(app_size > 0)
            {
                for(std::vector<app_build>::iterator it = loaded_apps.begin(); it != loaded_apps.end(); it++)
                {
                    app_build cur = *it;
                    if(procmessage::equals(cur.name, prerun))   //If it's an already loaded name get the executable path
                    {
                        namemap = cur.name;
                        prerun = cur.executable; //Set new executable
                        recent_apps.push_front(cur); //Add it to recent apps
                        found_desktop_app = true;
                        procmessage::run_save();
#ifdef DEBUG
                        std::cout << "DEBUG: Found executable by loaded name" << std::endl;
#endif // DEBUG
                        break;
                    }
                }
            }
#ifdef DEBUG
            else
            {

                std::cout << "DEBUG: Apps ended apparently there aren't any! Error must have occured" << std::endl;
            }
#endif // DEBUG
            if(!found_desktop_app)
            {
                app_build exec_app; //This isn't a desktop file so make a "temp app"
                std::string toname = namemap;
                if(procmessage::contains(toname, "/"))
                {
                    toname.substr(toname.rfind("/")); //If absolute change to just base name
                }

                if(procmessage::contains(toname, " "))
                {
                    toname.substr(0, toname.find(" ")); //We don't want args just the executable name
                }

                exec_app.name = toname;
                exec_app.executable = prerun;
                if(toname.length() > 0)
                {
                    std::string ton;
                    ton += toname[0];
                    exec_app.icon_path = boost::to_upper_copy<std::string>(ton);//There isn't any hopefully front end will put a default icon for this
                }
                else
                    exec_app.icon_path = "ERROR";
                recent_apps.push_front(exec_app); //Add it to recent apps
                procmessage::run_save();
            }

            std::string torun = run_pref + prerun; //Add prefix or endings here such as "xiwi"
#ifdef DEBUG
            std::cout << "DEBUG: RUN EXECUTABLE: " << torun << std::endl;
#endif
            procmessage::run_graphical(torun, namemap, s, hdl, msg);

        }
        else if(procmessage::equals(toexec, "getapps"))
        {
#ifdef DEBUG
            std::cout << "DEBUG: Getting apps" << std::endl;
#endif
            std::stringstream toret;
            toret << procmessage::get_apps();
            procmessage::send_message(s, hdl, msg, &toret);
        }
        else if(procmessage::equals(toexec, "updateapps"))
        {
#ifdef DEBUG
            std::cout << "DEBUG: Forcefully updating apps" << std::endl;
#endif // DEBUG
            procmessage::get_apps();

            std::stringstream okresp;
            okresp << jsoner::OkResponse();
            procmessage::send_message(s, hdl, msg, &okresp);
        }
        else if(procmessage::equals(toexec, "getrecent"))
        {
#ifdef DEBUG
            std::cout << "DEBUG: Getting recently used apps" << std::endl;
#endif // DEBUG
            int num_apps = (root.isMember("num")) ? root["num"].asInt() : (int)(maxRecentApps / 2);
            std::stringstream toret;
            toret << procmessage::get_recent_apps(num_apps);
            procmessage::send_message(s, hdl, msg, &toret);
        }
        else if(procmessage::equals(toexec, "searchapps"))
        {
#ifdef DEBUG
            std::cout << "DEBUG: Searching for apps" << std::endl;
#endif // DEBUG
            if(!root.isMember("search"))
            {
                std::stringstream tos;
                tos << jsoner::Error("Json parse error, search key failed", 1);
                procmessage::send_message(s, hdl, msg, &tos);
                return;
            }
            procmessage::run_search(root["search"].asString(), s, hdl, msg);
        }
        else if(procmessage::equals(toexec, "installapp"))
        {
#ifdef DEBUG
            std::cout << "DEBUG: Installing an app" << std::endl;
#endif // DEBUG
            if(!root.isMember("app"))
            {
                std::stringstream tos;
                tos << jsoner::Error("Json parse error, app key failed", 1);
                procmessage::send_message(s, hdl, msg, &tos);
                return;
            }
            procmessage::run_install(root["app"].asString(), s, hdl, msg);

        }
        else if(procmessage::equals(toexec, "removeapp"))
        {
#ifdef DEBUG
            std::cout << "DEBUG: Removing an app" << std::endl;
#endif // DEBUG
            if(!root.isMember("app"))
            {
                std::stringstream tos;
                tos << jsoner::Error("Json parse error, app key failed", 1);
                procmessage::send_message(s, hdl, msg, &tos);
                return;
            }
            procmessage::run_remove(root["app"].asString(), s, hdl, msg);

        }
        else if(procmessage::equals(toexec, "stopapp"))
        {
#ifdef DEBUG
            std::cout << "DEBUG: Stopping an app" << std::endl;
#endif // DEBUG
            if(!root.isMember("app"))
            {
                std::stringstream tos;
                tos << jsoner::Error("Json parse error, app key failed", 1);
                procmessage::send_message(s, hdl, msg, &tos);
                return;
            }
            std::string app = root["app"].asString();
            std::stringstream init_r = jsoner::StartAppKill(app);
            procmessage::send_message(s, hdl, msg, &init_r);
            std::stringstream response = procmessage::run_stop(app);
            procmessage::send_message(s, hdl, msg, &response);

        }
        else if(procmessage::equals(toexec, "getversion"))
        {
#ifdef DEBUG
            std::cout << "DEBUG: Sending version: " << current_version << std::endl;
#endif // DEBUG
            std::stringstream tosend = jsoner::VersionResponse();
            procmessage::send_message(s, hdl, msg, &tosend);
        }
        else
        {
#ifdef DEBUG
            std::cout << "DEBUG: Command " << toexec << " Not found" << std::endl;
#endif // DEBUG
            std::stringstream tos;
            tos << jsoner::Error("Command doesn't exist", 2);
            procmessage::send_message(s, hdl, msg, &tos);
        }
    }

    static void on_open(server *s, websocketpp::connection_hdl hdl)
    {
        std::cout << "CONNECTION OPENED" << std::endl;
    }

    static void on_close(server *s, websocketpp::connection_hdl hdl)
    {
        std::cout << "CONNECTION CLOSED" << std::endl;
    }

    static void on_fail(server *s, websocketpp::connection_hdl hdl)
    {
        std::cout << "CONNECTION FAILED" << std::endl;
    }
private:
    static bool initProc(const std::string &full, const std::string &finds, std::string *out = &blank)
    {
        if(boost::starts_with(full, finds))
        {
            *out = full.substr(finds.length());
            return true;
        }
        return false;
    }
};

#include "Imagehandle.hpp" //Add Image handle class

#endif
