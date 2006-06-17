// vim:tabstop=4:shiftwidth=4:noexpandtab:textwidth=80
/*
 * $RCSfile$
 *
 * $Log$
 * Revision 1.21  2006/06/17 16:48:38  hockm0bm
 * ObserverHandler::registerObserver bug fix
 *         - obsever is pushed if find returns == end()
 *
 * Revision 1.20  2006/06/17 15:40:07  misuj1am
 *
 * -- minor changes
 *
 * Revision 1.19  2006/06/17 15:06:20  hockm0bm
 * quick fix
 *         - <std_queue.h> include replaced by <queue>
 *
 * Revision 1.18  2006/06/17 13:15:29  hockm0bm
 * * PriorityList added
 * * PriorityComparator added
 * * IObserverHandler
 *         - supports priorities - uses PriorityList storage
 *         - registerObserver wih already registered obsever is ignored
 *
 * NEEDS testing
 *
 * Revision 1.17  2006/06/15 15:56:00  jahom0bm
 * Fixed include
 *
 * Revision 1.16  2006/06/13 20:44:13  hockm0bm
 * * ChangeContextType enum removed from class IChangeContext to observer namespace
 * * pdfwriter.cc synced with ChangeContextType change
 *
 * Revision 1.15  2006/06/12 18:28:14  hockm0bm
 * ComplexChangeContext class added
 *
 * Revision 1.14  2006/05/30 21:04:12  hockm0bm
 * doc update
 *
 * Revision 1.13  2006/05/30 10:51:08  misuj1am
 *
 * -- improved
 *
 * Revision 1.12  2006/05/16 17:43:58  hockm0bm
 * * ScopedChangeContext added
 *         - new change context
 *
 * Revision 1.11  2006/05/14 13:51:40  hockm0bm
 * quick fix
 *         - virtual destructor for IObserverHandler
 *
 * Revision 1.10  2006/05/14 13:29:00  hockm0bm
 * IObserverHandler has virtual methods
 *
 * Revision 1.9  2006/05/14 12:51:46  misuj1am
 *
 * -- implementation moved from iproperty.cc here
 *
 * Revision 1.8  2006/04/18 17:25:29  hockm0bm
 * IObserverHandler returned back
 *
 * Revision 1.7  2006/04/17 17:08:10  hockm0bm
 * throw() added to all methods which can't throw
 *
 * Revision 1.6  2006/04/16 23:04:53  misuj1am
 *
 *
 * -- slightly changed observer interface
 *
 * Revision 1.5  2006/04/12 17:58:28  hockm0bm
 * BasicChangeContext changed to BasicChangeContextType
 *
 * Revision 1.4  2006/04/09 21:32:03  misuj1am
 *
 *
 * -- changed a bit
 * 	-- virtual destructor
 * 	-- pointer to context changed to shared_ptr
 *
 * Revision 1.3  2006/04/02 08:09:59  hockm0bm
 * new observer design
 * * IObserver - interface for observer implementation (template class)
 * * IObserverHandler - interface for value keepers which wants to be
 *         registration target of observers (template class)
 * * IChangeContext - interface for change context for notifier (template class)
 *         - BasicCahngeContext - implementation of basic context with original
 *           value of changed one
 *
 *
 */

#ifndef _OBSERVER_H
#define _OBSERVER_H

#include <vector>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <queue>

//=============================================================================
namespace observer
{


/**
 * Observer exception.
 */
struct ObserverException : public std::exception
{
};

/** Supported context types.
 */
enum ChangeContextType {BasicChangeContextType, ComplexChangeContextType, ScopedChangeContextType};

/** Operator for human readable ChangeContextType printing.
 * @param str Stream where to print.
 * @param type Change context type.
 */
inline std::ostream & operator<<(std::ostream & str, ChangeContextType type)
{
	switch(type)
	{
		case BasicChangeContextType:
			str << "BasicChangeContextType";
			break;
		case ComplexChangeContextType:
			str << "ComplexChangeContextType";
			break;
		case ScopedChangeContextType:
			str << "ScopedChangeContextType";
			break;
	}
	return str;
}
	
/** Interface (pure abstract class) for change context.
 *
 * This is base class for all change contexts. It contains just information
 * about supported contexts (in enum) and type of context. 
 * <br>
 * User of this change context should check context type at first and than cast
 * to specific context subclass.
 */
template<typename T> class IChangeContext
{
public:
	
	/** Returns context type.
	 *
	 * @return context type.
	 */
	virtual ChangeContextType getType()const =0;

	/** Virtual destructor.
	 *
	 */
	virtual ~IChangeContext(){};
};

/** Basic change context template class.
 * 
 * Simplest subclass from IChangeContext. It holds previous value of changed
 * one. This value is wrapped by smart pointer to keep clear instancion policy.
 * <br>
 * Exception NOTE:
 * No method throws an exception.
 */
template<typename T> class BasicChangeContext:public IChangeContext<T>
{
	/** Original value. */
	boost::shared_ptr<T> originalValue;
public:
	/** Constructor with original value.
	 * @param origVal Original value used for originalValue initialization.
	 */
	BasicChangeContext(boost::shared_ptr<T> origVal)throw() :originalValue(origVal){};

	/** Destructor.
	 * Just to mark all destructors in subtypes as virtual.
	 */
	virtual ~BasicChangeContext()throw() {};

	/** Returns type of context.
	 *
	 * @return BasicChangeContextType value.
	 */
	ChangeContextType getType() const throw()
	{
		return BasicChangeContextType;
	}

	/** Returns original value.
	 *
	 * @return Orignal value wrapped by smart pointer.
	 */
	virtual boost::shared_ptr<T> getOriginalValue() const throw()
	{
		return originalValue;
	}
};

/** Complex change context template class.
 *
 * This context should be used when value with given ValueType template type is
 * changed in context of complex value type (it is its part) and there is unique
 * identification of this value.
 * <br>
 * Observer may use original value which is accessible from
 * BasicChangeContextType supertype and additional value identificator
 * information.
 */
template<typename ValueType, typename ValueIdType>
class ComplexChangeContext: public BasicChangeContext<ValueType>
{
	ValueIdType valueId;
public:
	/** Initialization constructor.
	 * @param origVal Original value.
	 * @param id Identificator of changed value.
	 * 
	 * Initializes originalValue (uses BasicChangeContext constructor with
	 * origVal parameter) and valueId from given id.
	 */
	ComplexChangeContext(boost::shared_ptr<ValueType> origValue, ValueIdType id):
		BasicChangeContext<ValueType>(origValue), valueId(id)
	{}

	virtual ~ComplexChangeContext()throw(){}

	/** Returns identificator of changed value.
	 *
	 * @return value identificator value.
	 */
	ValueIdType getValueId()const throw()
	{
		return valueId;
	}
	
	/** Returns type of context.
	 *
	 * @return ComplexChangeContextType value.
	 */
	ChangeContextType getType() const throw()
	{
		return ComplexChangeContextType;
	}
};

/** Change context with scope information.
 *
 * Scope information are specified as template parameter and describes scope of
 * the change. Template T parameter specifies value holder.
 * <br>
 * Scope can be rather complex information but principally it should contain
 * information which can be used to cathegorize value given in notify method as
 * newValue parameter (e. g. Maximum number of value for progress calculation
 * and newValue contains current state).
 * 
 */
template<typename T, typename S> class ScopedChangeContext:public IChangeContext<T>
{
	/** Scope holder.
	 */
	boost::shared_ptr<S> scope; 
public:
	/** Initialize constructor.
	 * @param s Scope used for initialization.
	 *
	 * Initializes scope field.
	 */
	ScopedChangeContext(boost::shared_ptr<S> s):scope(s){};

	/** Returns scope.
	 * @return Scope value wrapped by shared_ptr smart pointer.
	 */
	boost::shared_ptr<S> getScope()const
	{
		return scope;
	}

	/** Returns context type.
	 *
	 * @return Returns ScopedChangeContextType.
	 */
	ChangeContextType getType() const throw()
	{
		return ScopedChangeContextType;
	}

};


/** Observer interface (pure abstract class).
 *
 * Observer is mechanism how to be informed that somethig has changed. 
 * Implementator of this interface produces handler for this change event.
 * <br>
 * This should be used in following way:
 * <ul>
 * <li>value keeper which wants to enable observers has to implement
 * IObserverHandler interface which enables to register and unregister 
 * observers. It guaranties it calls notify on each registered observer after
 * change was registered.
 * <li>implementator of class is responsible for notify method implementation
 * which handles situation.
 * <li>observer must be registered on target value keeper.
 * </ul>
 *
 * Interface is template to enable different value types to be used (type 
 * parameter stands for value type to be managed - informed about change). 
 * This enables only one type to be handled by one implementator (with all 
 * subtypes).
 * <br>
 * Value change handling is done in notify method (see for more details). This
 * method is called after value has been changed. 
 * <br>
 * Each observer implementation has its priority which is used be value keeper
 * to determine order in which to notify obsevers, if there is more then one.
 * <br>
 * Exception NOTE:
 * No method throws an exception.
 */
template<typename T> class IObserver
{
public:
	/** Type for priority.
	 */
	typedef int priority_t;
	
	/** Notify method.
	 * @param newValue New value of changed value or its part.
	 * @param context Value change context.
	 * 
	 * Each time value keeper, which implements IObserverHandler, changes 
	 * value (or its part), all registered observers are notified about that
	 * by this method calling.
	 * <br>
	 * newValue stands for new value or its part (if value is complex). 
	 * <br>
	 * contex contains additional information about change. It depends on
	 * value keeper which information is provided (if any). It may be NULL
	 * (value stored in shared_ptr may be NULL - more precisely - use 
	 * contex.get()==NULL condition for checking) which means that no 
	 * information is provided. Method implementator should check context 
	 * type (using getType() method). Accoring this type cast to correct 
	 * IChangeContext subclass and use information.
	 * <p>
	 * <b>Example</b>:<br>
	 * Lets say that T is IProperty. If observer is registered on simple
	 * type, newValue contains new value of this simple one. If it is
	 * registered on complex type, it contains changed value (item) inside 
	 * this complex type (value keeper part is changed).
	 */
	virtual void notify (boost::shared_ptr<T> newValue, boost::shared_ptr<const IChangeContext<T> > context) const throw() = 0 ;

	/** Returns priority of obsever.
	 *
	 * Lower number means higher priority.
	 * @return Observer priority value.
	 */
	virtual priority_t getPriority()const throw() =0;

	/**
	 * Virtual destructor.
	 */
	virtual ~IObserver ()throw() {};
};

namespace {

/** Simple wapper for priority_queue with enabled iteration.
 *
 * Only purpose for this class is to provide us with iterable priority queue.
 * Implementation of STL priority_queue class doesn't enable elements itaration
 * but provide priority sorting functionality. As it has protected underlying
 * container, this class can access it and return iterators. Thi implementation
 * provide just const_iterator to protect from value changing and so discarding
 * priority queueing.
 * <br>
 * Template T parameter stands for stored element type, Storage is type of
 * underlying container type which is used by priority_queue superclass (see
 * priority_queue specification for requirements for underlying storage
 * container). Compare is functor for priority comparing (it is also used by 
 * priority_queue supertype).
 */
template<typename T, typename Storage=std::vector<T>, typename Compare=std::less<T> >
class PriorityList: public std::priority_queue<T, Storage, Compare>
{
public:
	/** Type for constant iterator.
	 */
	typedef typename Storage::const_iterator const_iterator;
	typedef typename std::priority_queue<T, Storage, Compare> PriorityQueue;

	/** Returns iterator for first element in queue.
	 * Iterator points to element with highest priority.
	 */
	const_iterator begin()const 
	{
		return PriorityQueue::c.begin();
	}

	/** Returns iterator behind last element in queue.
	 */
	const_iterator end()const
	{
		return PriorityQueue::c.end();
	}

	/** Returns constant itetor to element with same value.
	 * @param value Value to find.
	 *
	 * @return const_iterator for given value.
	 */
	const_iterator find(const T & value)const
	{
		for(const_iterator i=begin(); i!=end(); i++)
		{
			const T & elem=*i;
			if(elem==value)
				return i;
		}

		return end();
	}

	/** Removes given value from list.
	 * @param value Value to remove.
	 *
	 * Removes element from container and keeps priority ordering for other
	 * elements.
	 */
	void erase(const T & value)
	{
		for(typename Storage::iterator i=PriorityQueue::c.begin(); i!=PriorityQueue::c.end(); i++)
		{
			T & elem=*i;
			if(elem==value)
			{
				// removes iterator and consolidates underlying container
				PriorityQueue::c.erase(i);	
				std::make_heap(PriorityQueue::c.begin(), PriorityQueue::c.end(), PriorityQueue::comp);
				return;
			}
		}
	}
};

/** Priority comparator functor.
 *
 * T template parameter stands for elements which priority should be compared.
 * It has to provide getPriority method and has to be pointer compatible type
 * (pointer or kind of smart pointer).
 */
template <typename T>
struct PriorityComparator
{
	/** Comparator functor.
	 * @param value1 Value to compare.
	 * @param value2 Value to compare.
	 * 
	 * @return value1-&gt;getPriority() &lt; value2-&gt;getPriority()
	 */
	bool operator ()(const T & value1, const T & value2)const
	{
		return value1->getPriority() < value2->getPriority();
	}
};

} // annonymous namespace for priority list and related

/** Base class for all notifiers.
 *
 * Each class which want to support observers should inherit from this class. It
 * provides basic implementation for registering, unregistering and notification
 * of observers.
 * Whenever change occures, subclass is responsible to call notifyObservers
 * method and provide it with correct parameters. Rest is done by superclass.
 * <br>
 * T template parameter stands for value type which is observed (same as used
 * for IObserver template type). Observers are IObserver with T type wrapped 
 * by shared_ptr smart pointer.
 * <br>
 * This implementation supports priority ordering of observers.
 *
 */
// FIXME rename to ObserverHandler
template<typename T> class IObserverHandler
{
public:
	/** Type for observer.
	 * Alias to IObserver with T type wrapped by shared_ptr smart pointer.
	 */
	typedef boost::shared_ptr<const IObserver<T> > Observer; 

	/** Type for observer list.
	 * Alias to PriorityList with shared_ptr&lt;Observer&gt; value type,
	 * vector underlying container and PriorityComparator comparator.
	 */
	typedef PriorityList<Observer, std::vector<Observer>, PriorityComparator<Observer> > ObserverList;

	typedef IChangeContext<T>  ObserverContext;

	// FIXME remove - it shouldn't be here
	typedef BasicChangeContext<T> 	BasicObserverContext;
	typedef ComplexChangeContext<T,size_t>		CArrayComplexObserverContext;
	typedef ComplexChangeContext<T,std::string>	CDictComplexObserverContext;

protected:
	/** List of registered observers.
	 * It is iterable priority queue, so observers are ordered according their
	 * priorities (higher priority closer to begin - NOTE that higher priority
	 * means smaller priority value.
	 */
	ObserverList observers;
	
public:
	/** Empty destructor.
	 */
	virtual ~IObserverHandler(){}
	
	/** Registers new observer.
	 * @param observer Observer to register (if NULL, nothing is registered).
	 *
	 * Adds given observer to observers list. Ignores if observer is already in
	 * the list.
	 *
	 * @throw ObserverException if given observer is not valid (it is NULL).
	 */
	virtual void registerObserver(const Observer & observer)
	{
		if (observer.get())
		{
			// ignores if it is already in the list
			if(observers.find(observer)==observers.end())
				observers.push(observer);
		}
		else
			throw ObserverException ();

	}

	/** Unregisters given observer.
	 * @param observer Observer to unregister.
	 *
	 * Removes given observer from observers list. 
	 *
	 * @throw ObserverException if observer is not in observers list.
	 */
	virtual void unregisterObserver(const Observer & observer)
	{
		if (observer.get())
		{
			typename ObserverList::const_iterator it = observers.find(observer);

			if (it != observers.end ())
				observers.erase (observer);
			else
				throw ObserverException ();
		
		}else
			throw ObserverException ();
	}

	/**
	 * Notify all observers about a change.
	 *
	 * Observers are notified in according their priorities. Higher priority
	 * (smaller priority value) sooner it is called. Observers with same
	 * priorities are called in unspecified order. 
	 *
	 * @param newValue Object with new value.
	 * @param context Context in which the change has been made.
	 */
	virtual void notifyObservers (boost::shared_ptr<T> newValue, boost::shared_ptr<const ObserverContext> context)
	{
		// obsrvers list is ordered by priorities, so iteration works correctly
		typename ObserverList::const_iterator it = observers.begin ();
		for (; it != observers.end(); ++it)
			(*it)->notify (newValue, context);
	}
	
};


} // namespace observer




#endif  // _OBSERVER_H
