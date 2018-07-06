/*
 * Twirre: architecture for autonomous UAVs using interchangeable commodity components
 *
 * Copyright© 2017 Centre of expertise in Computer Vision & Data Science, NHL Stenden University of applied sciences
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CORE_OWNED_MUTEX_H_
#define CORE_OWNED_MUTEX_H_

#include <mutex>
#include <condition_variable>
#include <thread>

namespace twirre
{
	/**
	 * A mutex with the concept of 'ownership'. Similar to a recursive mutex, this mutex can be locked multiple times by the same thread. However,
	 * a single unlock suffices to fully unlock this mutex. Upon locking, the mutex will store the thread::id of the locking thread, which is then
	 * regarded as the 'owner' of the mutex. Only the owner can lock/unlock the mutex. Upon unlocking, ownership is lost.
	 */
	class owned_mutex
	{
	public:
		owned_mutex();
		virtual ~owned_mutex();

		/**
		 * Lock this mutex.
		 *
		 * If the mutex is currently not owned, the calling thread will take ownership of the mutex. If the mutex is owned by the calling thread itself,
		 * this operation will return without any effect. If the mutex is owned by a different thread, the calling thread will wait until ownership becomes
		 * available again, after which it will take ownership.
		 *
		 * This mutex can be locked multiple times, and only requires a single unlock to become fully unlocked again.
		 */
		void lock();

		/**
		 * Unlock this mutex.
		 *
		 * The mutex will be fully unlocked. This relinquishes ownership of the mutex, so a different thread is allowed to lock it.
		 * This operation will do nothing if this mutex is not currently owned by a thread.
		 *
		 * @throw std::logic_error when lock() is called from a different thread than the current owner.
		 */
		void unlock();

	private:
		std::mutex _mutex;
		std::condition_variable _waitLock;
		bool _isOwned;
		std::thread::id _ownerId;
	};
}
#endif /* CORE_OWNED_MUTEX_H_ */
