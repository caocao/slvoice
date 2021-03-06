/* connect_state.cpp -- connector state module
 *
 *			Copyright 2008, 3di.jp Inc
 */

#include "main.h"
#include "state.hpp"
#include "server_util.hpp"

#include <boost/lexical_cast.hpp>

//=============================================================================
// Connector Idle
//=============================================================================
ConnectorIdleState::ConnectorIdleState(my_context ctx) :
        my_base(ctx), // required because we call context() from a constructor
        machine(context<ConnectorMachine>()) 
{
	g_logger->Debug("STATE") << "ConnectorIdle entered" << endl;
}

ConnectorIdleState::~ConnectorIdleState() 
{
	g_logger->Debug("STATE") << "ConnectorIdle exited" << endl;
}

result ConnectorIdleState::react(const InitializeEvent& ev) 
{
	g_logger->Debug("STATE") << "ConnectorIdle react (InitializeEvent)" << endl;

	//machine.info->voiceserver_url = g_config->VoiceServerURI;
	//g_logger->Info() << "VoIP frontend URL = " << machine.info->voiceserver_url << endl;

    machine.info->handle = VFVW_CONNECTOR_HANDLE;

    ((ConnectorCreateResponse *)ev.result)->ConnectorHandle = machine.info->handle;
    //((ConnectorCreateResponse *)ev.result)->VersionID = "";	// TODO

    return transit<ConnectorActiveState>();
}

//=============================================================================
// Connector Active
//=============================================================================
ConnectorActiveState::ConnectorActiveState(my_context ctx) :
        my_base(ctx), // required because we call context() from a constructor
        machine(context<ConnectorMachine>()) 
{
	g_logger->Debug("STATE") << "ConnectorActive entered" << endl;
}

ConnectorActiveState::~ConnectorActiveState() 
{
	g_logger->Debug("STATE") << "ConnectorActive exited" << endl;
}

result ConnectorActiveState::react(const ShutdownEvent& ev) 
{
	g_logger->Debug("STATE") << "ConnectorActive react (ShutdownEvent)" << endl;

    machine.info->handle = "";
    g_logger->Terse("STATE") << "=======  CONNECT STATE  ======== Stop SIP" << endl;

    // Added 
    //SIPConference *psc;
    //if (psc != NULL) 
    //{
    //   psc->stop_sip_stack_();
    //}

    return transit<ConnectorIdleState>();
}

result ConnectorActiveState::react(const AudioEvent& ev) 
{
	g_logger->Debug("STATE") << "ConnectorActive react (AudioEvent)" << endl;

    ev.message->SetState(machine.info->audio);

	// control all sessions
	machine.info->session.controlAudioLevel();

    return discard_event();
}
