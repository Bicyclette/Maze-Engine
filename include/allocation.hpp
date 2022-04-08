#ifndef ALLOC_HPP
#define ALLOC_HPP

#include <vector>
#include <utility>

template <typename U>
class UniquePtr
{
	public:
		UniquePtr(U* ptr = nullptr) : m_ptr(ptr) {}
		UniquePtr(const UniquePtr<U>& ptr) = delete;
		UniquePtr(UniquePtr<U>&& ptr)
		{
			m_ptr = ptr.m_ptr;
			ptr.m_ptr = nullptr;
		}
		UniquePtr& operator=(const UniquePtr<U>& ptr) = delete;
		UniquePtr& operator=(UniquePtr<U>&& ptr)
		{
			if (&ptr == this)
				return *this;
			delete m_ptr;
			m_ptr = ptr.m_ptr;
			ptr.m_ptr = nullptr;
			return *this;
		}
		~UniquePtr() { delete(m_ptr); }
		U& operator*() const { return *m_ptr; };
		U* operator->() const { return m_ptr; };
	private:
		U* m_ptr;
};

template <typename S>
class SharedPtr
{
	public:
		inline static std::vector<std::pair<S*, int>> control;
	public:
		SharedPtr(S* ptr = nullptr) : m_ptr(ptr)
		{
			SharedPtr<S>::control.emplace_back(m_ptr, 1);
		}
		~SharedPtr()
		{
			for (int i{ 0 }; i < SharedPtr<S>::control.size(); ++i)
			{
				auto& mem = SharedPtr<S>::control[i];
				if (mem.first == m_ptr)
				{
					if (mem.second == 1)
					{
						SharedPtr<S>::control.erase(SharedPtr<S>::control.begin() + i);
						delete m_ptr;
					}
					else
						mem.second--;
				}
			}
		}
		SharedPtr(const SharedPtr<S>& ptr)
		{
			m_ptr = ptr.m_ptr;
			for (auto& mem : SharedPtr<S>::control)
			{
				if (mem.first == m_ptr)
					mem.second++;
			}
		}
		SharedPtr& operator=(const SharedPtr<S>& ptr)
		{
			if (&ptr == this)
				return *this;
			for (int i{ 0 }; i < SharedPtr<S>::control.size(); ++i)
			{
				auto& mem = SharedPtr<S>::control[i];
				if (mem.first == m_ptr)
				{
					if (mem.second == 1)
					{
						SharedPtr<S>::control.erase(SharedPtr<S>::control.begin() + i);
						delete m_ptr;
					}
					else
						mem.second--;
				}
			}
			m_ptr = ptr.m_ptr;
			for (auto& mem : SharedPtr<S>::control)
			{
				if (mem.first == m_ptr)
					mem.second++;
			}
		}
		S& operator*() const { return *m_ptr; };
		S* operator->() const { return m_ptr; };
	private:
		S* m_ptr;
};

#endif