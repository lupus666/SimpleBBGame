#ifndef _Client_H_
#define _Client_H_

#include<memory>
#include<iostream>
#include<string>

#include<grpcpp\grpcpp.h>
#include<grpc\support\log.h>

#include"Classes\Network\player.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using player::SingePlayerInfo;
using player::MapInfo;
using player::Player;

class Client {
public:
	explicit Client(std::shared_ptr<Channel> channel)
		:_stub(Player::NewStub(channel)){}

	MapInfo sendPlayerInfo(const SingePlayerInfo& playerInfo)
	{
		SingePlayerInfo request;
		request = playerInfo;

		MapInfo reply;

		ClientContext context;

		CompletionQueue cq;

		Status status;

		std::unique_ptr<ClientAsyncResponseReader<MapInfo>>rpc(_stub->PrepareAsyncPlayerInfo(&context, request, &cq));

		rpc->StartCall();

		rpc->Finish(&reply, &status, (void*)1);

		void* got_tag;
		bool ok = false;

		GPR_ASSERT(cq.Next(&got_tag, &ok));

		GPR_ASSERT(got_tag == (void*)1);

		GPR_ASSERT(ok);

		if (status.ok())
		{
			return reply;
		}

	}

private:

	std::unique_ptr<Player::Stub> _stub;

};

#endif // !_Client_H_

