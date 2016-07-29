#pragma once

struct RenderMessage;

class Subscriber
{
public:
	Subscriber();
	~Subscriber();

	virtual void ReceiveMessage(const RenderMessage& aMessage);
};

