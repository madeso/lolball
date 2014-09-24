#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include "Container.hpp"

template<class Type>
class Data : public Container {
public:
	Data(const std::string& pName, Type pData) : Container(pName), mData(pData) {
	}
	Data(const std::string& pName) : Container(pName), mData() {
	}
	const Type& get() const {
		return mData;
	}
	Type& getForModification() {
		return mData;
	}
	void set(const Type& pData) {
		mData = pData;
		handleOnChange();
	}

	Type* operator->() const {
		return &mData;
	}
	const Type& operator*() const {
		return &mData;
	}
	Type* operator->() {
		return &mData;
	}
	Type& operator*() {
		return mData;
	}
private:
	Type mData;
};

#endif