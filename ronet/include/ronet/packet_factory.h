/* $Id$ */
/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2009 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#ifndef __RONET_PACKET_FACTORY_H
#define __RONET_PACKET_FACTORY_H

#include "packet_vector.h"
#include "packet.h"
#include "buffer.h"
#include "connection.h"

#include <vector>
#include <map>

namespace ronet {
	/**
	 * The maker of all (incoming) packets.
	 * Transforms received data input into packet classes for easy handling
	 */
	class RONET_DLLAPI PacketFactory {
	private:
		/** Packets in the queue */
		PacketVector packets;
	protected:

		/** Handler function definition */
		typedef bool (PacketFactory::*Handler)(ucBuffer& p);

		/**
		 * 
		 */
		class RONET_DLLAPI Dispatcher {
		private:
			std::map<unsigned short, Handler> m_calls;
		protected:
			PacketFactory* m_factory;
		public:
			Dispatcher(PacketFactory*);

			void Register(unsigned short, Handler);
			bool Call(unsigned short, ucBuffer&);
		};

#define DECL(x) bool Handle_ ##x (ucBuffer&)

		//Add new packets here
		DECL(HpUpdateParty);
		DECL(OtherSpeech);
		DECL(CharLeaveScreen);
		DECL(GmBroad);
		DECL(ServerTick);
		DECL(AttackRange);
		DECL(DisplayStat);
		DECL(UpdateStatus);
		DECL(ServerList);
		DECL(CharCreateError);
		DECL(CharList);
		DECL(LoginError);
		DECL(AuthFailed);
		DECL(CharCreated);
		DECL(CharPosition);
		DECL(MapAcctSend);
		DECL(MapLoginSuccess);
		DECL(OwnSpeech);
		DECL(SkillList);
		DECL(MapMoveOk);
		DECL(PlayerEquip);
		DECL(ItemGained);
		DECL(ItemLost);
		DECL(InventoryItems);
		DECL(InventoryItemsStackable);
		//DECL(StatInfo);
		//DECL(StatInfo2);
		DECL(HotkeyList);
		DECL(ZenyExp);
		DECL(StatsInfo);
		DECL(ActorDisplay);
		DECL(ActorSpawn);
		DECL(ActorWalking);
		DECL(RecvNpcTalk);
		DECL(RecvNpcTalkNext);
		DECL(RecvNpcTalkClose);
		DECL(RecvNpcTalkResponses);
		DECL(RecvNpcInputReq);
		DECL(RecvNpcImage2);
		DECL(MapChange);
		DECL(ActorMove);
		DECL(ActorAction);
		DECL(ActorStop);
		DECL(RestartCharSelect);
		DECL(StatusUpAck);
		DECL(Emotion);
		DECL(NotifyEffect1);
		DECL(QuitAck);

		// Guild
		//DECL(GuildAlliesEnemiesList);
		DECL(GuildMasterMember);
		//DECL(GuildEmblem);
		//DECL(GuildMembersList);
		//DECL(GuildMemberPositionChanged);
		//DECL(GuildLeave);
		//DECL(GuildExpulsion);
		DECL(GuildBroken);
		//DECL(GuildMemberSettingList);
		//DECL(GuildSkillsList);
		//DECL(GuildExpulsionList);
		//DECL(GuildMembersTitleList);
		DECL(GuildCreateResult);
		DECL(GuildInviteResult);
		//DECL(GuildRequest);
		//DECL(GuildName);
		DECL(GuildMemberOnlineStatus);
		//DECL(GuildNotice);
		//DECL(GuildAllyRequest);
		DECL(GuildAlliance);
		//DECL(GuildPositionChanged);
		DECL(GuildMessage);
		DECL(GuildOppositionResult);
		DECL(GuildUnally);
		//DECL(GuildAllianceAdded);
		//DECL(GuildInfo);
		DECL(GuildLocation);
		//DECL(GuildMemberOnlineStatus2);

		// Pet
		//DECL(PetCaptureProcess);
		DECL(PetCaptureResult);
		DECL(PetFood);
		//DECL(PetInfo);
		DECL(PetInfo2);
		DECL(PetEmotion);
		//DECL(EggList);

		void push(Packet*);

		Dispatcher m_dispatcher;

	public:
		PacketFactory();
		~PacketFactory();

		unsigned int count() const;
		Packet* pop();

		bool generatePackets(ucBuffer&);
		PacketFactory& operator << (ucBuffer&);
		bool operator << (Connection&);
	};
}

#endif /* __RONET_PACKET_FACTORY_H */
