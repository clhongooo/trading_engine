#ifndef _ATU_THREADSAFEQUEUE_
#define _ATU_THREADSAFEQUEUE_
#include <string>
using namespace std;

template <class T>
class ATU_ThreadSafeQueue {
public:
	ATU_ThreadSafeQueue(int size) //: m_size(0)
	{
		m_queue=new boost::lockfree::queue< T >(size);
	}
    ~ATU_ThreadSafeQueue()
	{
		T in;
		while (m_queue->pop(in)) {
			delete in;
		}
		delete m_queue;
		m_queue=NULL;
	}
	void push(T in) {
		try {
			if (m_queue!=NULL) {
				m_queue->push(in);
			}
		}
		catch(std::exception &e)
		{
			cout << "ATU_ThreadSafeQueue push error" << std::endl;
		}
	}
	void pop(T &in) {
		try {
			if (m_queue!=NULL) {
				m_queue->pop(in);
			}
		}
		catch(std::exception &e)
		{
			cout << "ATU_ThreadSafeQueue pop error" << std::endl;
		}


	}
	bool empty() {
		return m_queue->empty();
	}
	boost::lockfree::queue< T > *m_queue;
//	int m_size;
};

#endif //_ATU_THREADSAFEQUEUE_
