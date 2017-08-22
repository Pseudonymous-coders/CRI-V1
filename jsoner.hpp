class jsoner
{
public:
    static inline std::string Error(const std::string &errorname, int error_codes)
    {
        return "{\"error\":\"" + errorname + "\",\"code\":" + boost::lexical_cast<std::string>(error_codes) + "}";
    }

    static inline std::string ErrorApp(const std::string &appname, const std::string &error)
    {
        return "{\"apperror\":\"" + appname  + "\", \"error\":\"" + error + "\"}";
    }

    static inline std::string ResponseType(const std::string &notify)
    {
        return "{\"response\": \"" +  notify + "\"}";
    }

    static inline std::string OkResponse()
    {
        return jsoner::ResponseType("ok");
    }

    static inline std::stringstream VersionResponse()
    {
        std::stringstream toresp;
        toresp << "{\"version\":{\"num\":\"" << current_version;
        toresp << "\",\"release\": \"" << current_release;
        toresp << "\",\"date\":\"" << current_date << "\"}}";

        return toresp;
    }

    static inline std::string UpdateGUI(const std::string &nameofapp)
    {
        return "{\"update\":\"" + nameofapp +"\"}";
    }

    static inline std::string AppBuild(const std::string &name,
                                       const std::string &description,
                                       const std::string &iconname,
                                       const std::string &execpath)
    {
        return "{\"name\": \"" + name + "\", \"desc\": \"" + description + "\", \"icon\": \"" + iconname
               + "\",\"exec\":\"" + execpath + "\"}";
    }

    static inline std::string SearchBuild(const std::string &name, const std::string &desc)
    {
        return "{\"name\":\"" + name + "\",\"desc\":\"" + desc + "\"}";
    }

    static std::string ApplicationsUpdate(const std::string &type, std::vector<app_build> applications)
    {
        std::string tocomp = "{\"" + type + "\":[";
        size_t numapps = applications.size();
        for(size_t ind = 0; ind < numapps; ind++)
        {
            app_build curapp = applications[ind];
            tocomp += AppBuild(curapp.name, curapp.comment, curapp.icon_path, curapp.executable) + ",";
        }
        tocomp = tocomp.substr(0, tocomp.size() - 1); //Remove last character which is a comma
        tocomp += "],\"num\":" + boost::lexical_cast<std::string>(numapps) + "}";
        return tocomp;
    }

    static std::string SearchUpdate(std::vector<search_build> applications)
    {
        std::string tocomp = "{\"search\":[";
        size_t numapps = applications.size();
        for(size_t ind = 0; ind < numapps; ind++)
        {
            search_build curapp = applications[ind];
            tocomp += SearchBuild(curapp.name, curapp.desc) + ",";
        }
        tocomp = tocomp.substr(0, tocomp.size() - 1); //Remove last character which is a comma
        tocomp += "],\"num\":" + boost::lexical_cast<std::string>(numapps) + "}";
        return tocomp;
    }

    static std::stringstream LogUpdate(const std::string &log)
    {
        std::stringstream tolog;
        tolog << "{\"log\": \"" << log << "\"}";
        return tolog;
    }

    static std::stringstream StartAppRun(const std::string &torun)
    {
        std::stringstream run;
        run << "{\"startapp\": \"" << torun << "\"}";
        return run;
    }

    static std::stringstream StartAppInstall(const std::string &toinstall)
    {
        std::stringstream install;
        install << "{\"startinstall\": \"" << toinstall << "\"}";
        return install;
    }

    static std::stringstream StartAppRemove(const std::string &toremove)
    {
        std::stringstream remover;
        remover << "{\"startremove\": \"" << toremove << "\"}";
        return remover;
    }

    static std::stringstream UpdateAppRun(const std::string &notify)
    {
        std::stringstream run;
        run << "{\"updateapp\": \"" << notify << "\"}";
        return run;
    }

    static std::stringstream UpdateAppInstall(const std::string &toinstall, const std::string &line)
    {
        std::stringstream install;
        install << "{\"updateinstall\": \"" << toinstall << "\",\"line\":\"" << line << "\"}";
        return install;
    }

    static std::stringstream UpdateAppRemove(const std::string &toremove, const std::string &line)
    {
        std::stringstream remover;
        remover << "{\"updateremove\": \"" << toremove << "\",\"line\":\"" << line << "\"}";
        return remover;
    }

    static std::stringstream ConnectedAppRun(const std::string &notify)
    {
        std::stringstream run;
        run << "{\"connectedapp\": \"" << notify << "\"}";
        return run;
    }

    static std::stringstream DoneAppInstall(const std::string &toinstall)
    {
        std::stringstream install;
        install << "{\"doneinstall\": \"" << toinstall << "\"}";
        return install;
    }

    static std::stringstream DoneAppRemove(const std::string &toremove)
    {
        std::stringstream remover;
        remover << "{\"doneremove\": \"" << toremove << "\"}";
        return remover;
    }

    static std::stringstream StartAppKill(const std::string &tokill)
    {
        std::stringstream proc_k;
        proc_k << "{\"startstop\": \"" << tokill << "\"}";
        return proc_k;
    }

    static std::stringstream EndAppKill(const std::string &tokill)
    {
        std::stringstream proc_k;
        proc_k << "{\"donestop\": \"" << tokill << "\"}";
        return proc_k;
    }

    static std::stringstream ErrorAppKill(const std::string &tokill, const std::string &error_line)
    {
        std::stringstream proc_k;
        proc_k << "{\"errorstop\": \"" << tokill << "\",\"line\":\"" << error_line << "\"}";
        return proc_k;
    }

    static std::stringstream ErrorAppInstall(const std::string &toinstall)
    {
        std::stringstream install;
        install << "{\"errorinstall\": \"" << toinstall << "\"}";
        return install;
    }

    static std::stringstream ErrorAppRemove(const std::string &toremove)
    {
        std::stringstream remover;
        remover << "{\"errorremove\": \"" << toremove << "\"}";
        return remover;
    }

    static std::stringstream EndAppRun(const std::string &notify, bool okay)
    {
        std::stringstream run;
        run << "{\"endapp\":\"" << notify << "\",\"exitok\":";
        if(okay) run << "true";
        else run << "false";
        run << "}";
        return run;
    }
};
