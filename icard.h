#ifndef CARD
#define CARD
#include <memory>
#include <string>

class ICard
{
public:
	virtual ~ICard() {}
	virtual int indexId() const = 0;
	virtual int typeId() const = 0;
	virtual int faceId() const = 0;
	virtual int clrId() const = 0;
	virtual bool isEmpty() const = 0;
	virtual std::string Name() const = 0;
};

using ICardPtr = std::shared_ptr<ICard>;

#endif // CARD

