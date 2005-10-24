/*
 * Oreka -- A media capture and retrieval platform
 * 
 * Copyright (C) 2005, orecx LLC
 *
 * http://www.orecx.com
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License.
 * Please refer to http://www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef __SIPSESSION_H__
#define __SIPSESSION_H__

#include "SipSession.h"
#include "Rtp.h"
#include <map>
#include "ace/Singleton.h"

class SipInviteInfo
{
public:
	struct in_addr m_fromIp;
	CStdString m_fromRtpPort;
	CStdString m_from;
	CStdString m_to;
	CStdString m_callId;
};
typedef boost::shared_ptr<SipInviteInfo> SipInviteInfoRef;

class SipByeInfo
{
public:
	CStdString m_callId;
};


class SipSession
{
public:
	SipSession();
	void Stop();
	void Start();
	void AddRtpPacket(RtpPacketInfoRef& rtpPacket);
	void ReportSipInvite(SipInviteInfoRef& invite);

	CStdString m_ipAndPort;
	SipInviteInfoRef m_invite;
	time_t m_lastUpdated;
private:
	void ProcessMetadata(RtpPacketInfoRef&);
	void ProcessMetadataIncoming();
	void ProcessMetadataOutgoing();
	void ReportMetadata();

	RtpPacketInfoRef m_lastRtpPacket;
	RtpRingBuffer m_rtpRingBuffer;
	struct in_addr m_invitorIp;
	struct in_addr m_inviteeIp;
	LoggerPtr m_log;
	CStdString m_capturePort;
	CStdString m_localParty;
	CStdString m_remoteParty;
	CaptureEvent::DirectionEnum m_direction;
};
typedef boost::shared_ptr<SipSession> SipSessionRef;

class SipSessions
{
public:
	SipSessions();
	void Stop(SipSessionRef& session);
	void ReportSipInvite(SipInviteInfoRef& invite);
	void ReportSipBye(SipByeInfo bye);
	void ReportRtpPacket(RtpPacketInfoRef& rtpPacket);
	void Hoover(time_t now);
private:
	std::map<CStdString, SipSessionRef> m_byIpAndPort;
	std::map<CStdString, SipSessionRef> m_byCallId;
	LoggerPtr m_log;
};
typedef ACE_Singleton<SipSessions, ACE_Thread_Mutex> SipSessionsSingleton;

#endif
