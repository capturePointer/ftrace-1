#ifndef RROM_UTILS_OBSERVER_HPP
#define RROM_UTILS_OBSERVER_HPP

#include <typeinfo>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <nullptr.hpp>
#include <RromUtils.hpp>
#include <rromLogger.hpp>

namespace Rrom
{

namespace Utils
{

template<typename> class Observer;

template<typename T> class IObservable
{
public:
    virtual ~IObservable() { }

    void handle(const Observer<T>& p_subject) { handleImpl(p_subject); }

    const std::string& identify() const { return identifyImpl(); }

protected:
    IObservable() { }

private:
    virtual void handleImpl(const Observer<T>& p_subject) = 0;

    virtual const std::string& identifyImpl() const = 0;
};

template<typename T> class Observer
{
    typedef boost::shared_ptr< IObservable<T> > IObserverType;
    typedef boost::shared_ptr< Rrom::Detail::IComparator<T> > IComparerType;
    typedef std::map<IObserverType, IComparerType> ObserversMap;

public:
    Observer() { }

    virtual ~Observer() { }

    virtual bool add(IObserverType p_observer, IComparerType p_comparer = IComparerType(new Rrom::Detail::DummyComparator<T>()))
    {
        if (!findObserverAndComparator(p_observer, p_comparer))
        {
            m_observers[p_observer] = p_comparer;
            CPLANE_DEBUG(__LINE__) << "New observer added: " << p_observer->identify();
            return true;
        }

        return false;
    }

    virtual bool remove(const std::string& p_observerId)
    {
        for(typename ObserversMap::iterator l_it = m_observers.begin();
            l_it != m_observers.end(); ++l_it)
        {
           if (l_it->first->identify() == p_observerId)
           {
               m_observers.erase(l_it);
               return true;
           }
        }

        return false;
    }

    virtual bool notify() const
    {
        CPLANE_DEBUG(__LINE__) << "Observer notify executed";

        if (!m_obj.get())
        {
            CPLANE_WARN(__LINE__) << "Observer object not set";
            return false;
        }

        for(typename ObserversMap::const_iterator l_it = m_observers.begin();
            l_it != m_observers.end(); ++l_it)
        {
            if (l_it->second != nullptr && (*l_it->second)((*m_obj)))
            {
                CPLANE_DEBUG(__LINE__) << "Observer: " << l_it->first->identify()
                                      << " will be notified";

                l_it->first->handle(*this);
            }
        }

        return true;
    }

    virtual const T* get() const
    {
        return m_obj.get();
    }

    virtual void set(std::auto_ptr<T> p_obj)
    {
        assert((p_obj.get() && "Observer null object"));
        m_obj = p_obj;
    }

private:
    bool findObserverAndComparator(IObserverType p_observer, IComparerType p_comparer) const
    {
        for(typename ObserversMap::const_iterator l_it = m_observers.begin();
            l_it != m_observers.end(); ++l_it)
        {
            if ((typeid(*p_observer) == typeid(*l_it->first)) &&
                (typeid(*p_comparer) == typeid(*l_it->second)))
            {
                CPLANE_WARN(__LINE__) << "The same observer exists and will not be added second time: "
                                      << p_observer->identify();
                return true;
            }
        }

        return false;
    }

    std::map<IObserverType, IComparerType> m_observers;
    std::auto_ptr<T> m_obj;
};

} // namespace Utils

} // namespace Rrom

#endif

