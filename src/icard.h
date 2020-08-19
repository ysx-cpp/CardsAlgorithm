#ifndef ICARD_H
#define ICARD_H
#include <memory>
#include <string>

class ICard
{
public:
	virtual ~ICard() {}
	virtual int IndexId() const = 0;
	virtual int TypeId() const = 0;
	virtual int FaceId() const = 0;
	virtual int ClrId() const = 0;
	virtual bool IsEmpty() const = 0;
	virtual std::string Name() const = 0;
};

using ICardPtr = std::shared_ptr<ICard>;

#endif // ICARD_H

