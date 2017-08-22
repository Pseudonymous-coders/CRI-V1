#include "config.h"
#include "message_handle.hpp"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

int main(int argc, char** argv)
{
    //packageimage::DownloadImage("10B4_steam.0.png", "/root/Downloads/steam.png");
    //themes::get_current_theme();
    //bool there = themes::has_icon("NoMachine-recording");
    //std::cout << "YES: " << there << std::endl;
    //procmessage::get_apps();
    //std::cout << "CURPATH: " << system::get_path() << std::endl;
    if(argc > 1)
    {
        for(int ind = 1; ind < argc; ind++)
        {
            std::string arg(argv[ind]);
            if(boost::starts_with(arg, pname))
            {
                try
                {
                    port = boost::lexical_cast<unsigned int>(arg.c_str()+plen);
                }
                catch(boost::bad_lexical_cast)
                {
                    std::cerr << "Port assignment wasn't a positive integer" << std::endl;
                    exit(1);
                }
            }
            else
            {
                std::cerr << "Unrecongnized argument: " << arg << std::endl;
                exit(1);
            }
        }
    }
#ifdef DEBUG
    std::cout << "Starting criserver on port: " << port << std::endl << "Created by: David Smerkous and Eli Smith" << std::endl;
    std::cout << "Initializing..." << std::endl;
#endif // DEBUG
    try
    {
        system::set_path();
    }
    catch(...) {}

    try
    {
        procmessage::run_load(); //Load recent apps settings
    }
    catch(...) {}

    server criserver;
    try
    {
        //Server init
        criserver.set_access_channels(websocketpp::log::alevel::all);
        criserver.clear_access_channels(websocketpp::log::alevel::frame_payload);
        criserver.set_reuse_addr(true); //Reuse already bound address
        criserver.init_asio();

        // Set the message handler
        criserver.set_message_handler(bind(&messagehandler::on_message,&criserver,::_1,::_2));

        // Set the on open handler
        criserver.set_open_handler(bind(&messagehandler::on_open, &criserver, ::_1));

        // Set the on close handler
        criserver.set_close_handler(bind(&messagehandler::on_close, &criserver, ::_1));

        // Set the on fail handler
        criserver.set_fail_handler(bind(&messagehandler::on_fail, &criserver, ::_1));

        // Listen on port specified in config
        criserver.listen(port);

        // Start the server accept loop
        criserver.start_accept();

        // Start the ASIO io_service run loop
        criserver.run();

    }
    catch (websocketpp::exception const & e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cout << "CRISERVER fatal error in websocket init" << std::endl;
        return 1;
    }
    return 0;
}
