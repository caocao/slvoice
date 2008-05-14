/* server.hpp -- server definition
 *
 *			Ryan McDougall -- 2008
 */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <sockets/Sockets.h>

const int glb_default_port (44124);

//=============================================================================
// Server class

class Server
{
    public:
        Server (int port = glb_default_port);
        ~Server ();
        
        void Start ();
        void Send (const string&);
        void Conference (const string&);
        void Conference (const Session&);

        StateMachine& GetStateMachine () { return state_; }

    private:
        void enqueue_request_ (char* mesg);
        void process_request_queue_ ();
        void flush_messages_on_event_ (Event& ev);

    private:
        const int port_;
        const size_t bufsize_;
        auto_ptr <char> buf_;

        TCPSocketWrapper server_;
        auto_ptr <TCPSocketWrapper> sock_;
        
        RequestQueue queue_;

    private:
        StateMachine state_;
        auto_ptr <SIPConference> activeconference_;

    private:
        Server (const Server&);
        void operator= (const Server&);
};

#endif //_SERVER_HPP_
