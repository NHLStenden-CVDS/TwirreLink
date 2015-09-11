/*
 * OwnedMutex.cpp
 *
 *  Created on: Jun 26, 2015
 *      Author: m-r-d
 */

#include "owned_mutex.h"
#include <stdexcept>


namespace twirre
{

	owned_mutex::owned_mutex() : _isOwned(false)
	{

	}

	owned_mutex::~owned_mutex()
	{

	}

	void owned_mutex::lock()
	{
		//prepare a lock for the internal mutex
		std::unique_lock<std::mutex> lock(_mutex);

		//return if this owned_mutex is already owned by the calling thread
		if(_isOwned && (std::this_thread::get_id() == _ownerId))
			return;

		//wait until this owned_mutex is owner-free
		_waitLock.wait(lock, [this](){return !_isOwned;});

		//update ownership
		_ownerId = std::this_thread::get_id();
		_isOwned = true;
	}

	void owned_mutex::unlock()
	{
		//protected scope
		{
			//lock the internal mutex
			std::lock_guard<std::mutex> lock(_mutex);

			//return if this owned_mutex is currently not owned
			if(!_isOwned)
				return;

			//throw exception if the current thread does not own this mutex
			if(std::this_thread::get_id() != _ownerId)
				throw std::logic_error("cannot unlock an owned_mutex which is owned by another thread");

			//clear ownership
			_isOwned = false;
		}

		//notify one waiting thread
		_waitLock.notify_one();
	}
}
