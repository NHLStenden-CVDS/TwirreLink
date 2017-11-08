 /*
  * Copyright © 2017, Centre of Expertise Computer Vision & Data Science, NHL Stenden university of applied sciences
  * All rights reserved.
  *
  * no warranty, no liability
  */

#ifndef CORE_OWNED_MUTEX_H_
#define CORE_OWNED_MUTEX_H_

#include <mutex>
#include <condition_variable>
#include <thread>

namespace twirre
{
	class owned_mutex
	{
	public:
		owned_mutex();
		virtual ~owned_mutex();

		void lock();
		void unlock();

	private:
		std::mutex _mutex;
		std::condition_variable _waitLock;
		bool _isOwned;
		std::thread::id _ownerId;
	};
}
#endif /* CORE_OWNED_MUTEX_H_ */
