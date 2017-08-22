//Get default libs from config (Included at bottom of message_handle
//Just to seperate the headers and make it look cleaner

#define WORKING_I_DIR "icons/"
#define EXT_TYPE ".png"
#define DOT_EXT "."
#define BLANK ""

using namespace boost::asio;
using boost::asio::ip::tcp;

class packageimage {
    public:
        //Checks to see if the (type: png) file exists on the system according to root path
        static bool contains(const std::string &name) {
            std::string tocontain = icondir + name;
            if(procmessage::contains(name, DOT_EXT)) {
                tocontain = name.substr(0, name.rfind(DOT_EXT));
            }
            tocontain += EXT_TYPE;
            return procmessage::filetest(tocontain);
        }

        static std::string DownloadFileBack(const std::string& serverName, const std::string& path) {
            std::string compile = "wget --no-check-certificate \"" + serverName + "\"" + " -O \"" + path + "\"";
#ifdef DEBUG
            std::cout << "DEBUG: DOWNLOADING FILE: " << compile << std::endl;
#endif //DEBUG
            simpleproc ret = system::simple(compile);
            std::string retName = "";
            for(std::string line : ret.second)
                retName += line;
#ifdef DEBUG
            std::cout << "DEBUG: DONE DOWNLOADING: (CODE: " << ret.first << "): " << retName << std::endl;
#endif // DEBUG
            return retName;
        }

        static std::string DownloadImage(const std::string &image, const std::string &path) {
            return packageimage::DownloadFileBack(gitbase + iconbase + image, path);
        }
};
