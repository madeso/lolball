#ifndef DATA_POINTER_HPP
#define DATA_POINTER_HPP

#include <string>
#include <cassert>
#include "ContainerPointer.hpp"

#include "Data.hpp"

template<class Type>
class DataPointer : public ContainerPointer {
public:
	DataPointer(const std::string& pName) : ContainerPointer(pName) {
		mOwner = dynamic_cast< Data<Type>* >( getOwner() );
		assert(mOwner && "Type mismatch between owner and slave");
	}
	void onChange() {
		//std::cout << "On Change: " << get() << std::endl;
	}
	const Type& get() const {
		assert(mOwner);
		return mOwner->get();
	}
	const Type* operator->() const {
		assert(mOwner);
		return &(mOwner->get());
	}
	const Type& operator*() const {
		assert(mOwner);
		return mOwner->get();
	}
	Type* operator->() {
		assert(mOwner);
		return &(mOwner->getForModification());
	}
	Type& operator*() {
		assert(mOwner);
		return mOwner->getForModification();
	}
	void set(const Type& pData) {
		assert(mOwner);
		mOwner->set(pData);
	}
private:
	Data<Type>* mOwner;
};

#endif