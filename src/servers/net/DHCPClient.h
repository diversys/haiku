/*
 * Copyright 2006-2010, Haiku, Inc. All Rights Reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Axel Dörfler, axeld@pinc-software.de
 */
#ifndef DHCP_CLIENT_H
#define DHCP_CLIENT_H


#include "AutoconfigClient.h"

#include <netinet/in.h>

class BMessageRunner;
class dhcp_message;

enum dhcp_state {
	INIT,
	REQUESTING,
	BOUND,
	RENEWAL,
	REBINDING,
	ACKNOWLEDGED,
};


class DHCPClient : public AutoconfigClient {
public:
								DHCPClient(BMessenger target,
									const char* device);
	virtual						~DHCPClient();

	virtual	status_t			Initialize();

	virtual	void				MessageReceived(BMessage* message);

private:
			status_t			_Negotiate(dhcp_state state);
			void				_ParseOptions(dhcp_message& message,
									BMessage& address,
									BMessage& resolverConfiguration);
			void				_PrepareMessage(dhcp_message& message,
									dhcp_state state);
			status_t			_SendMessage(int socket, dhcp_message& message,
									sockaddr_in& address) const;
			dhcp_state			_CurrentState() const;
			void				_ResetTimeout(int socket, time_t& timeout,
									uint32& tries);
			bool				_TimeoutShift(int socket, time_t& timeout,
									uint32& tries);
			void				_RestartLease(bigtime_t lease);
			BString				_ToString(const uint8* data) const;
			BString				_ToString(in_addr_t address) const;

private:
			BMessage			fConfiguration;
			BMessage			fResolverConfiguration;
			BMessageRunner*		fRunner;
			uint8				fMAC[6];
			uint32				fTransactionID;
			in_addr_t			fAssignedAddress;
			sockaddr_in			fServer;
			bigtime_t			fStartTime;
			bigtime_t			fRenewalTime;
			bigtime_t			fRebindingTime;
			bigtime_t			fLeaseTime;
			status_t			fStatus;
};

#endif	// DHCP_CLIENT_H
