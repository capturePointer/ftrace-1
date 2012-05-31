#ifndef FSTUB_ACTIONS_HPP
#define FSTUB_ACTIONS_HPP

namespace ftrace {

template<typename Ret>
class action
{
public:
    virtual Ret invoke() = 0;
};

template<typename Ret>
class returnable_action : public action<Ret>
{
public:
    explicit returnable_action(Ret returnable)
        : returnable_(returnable)
    { }

    Ret invoke() { return returnable_; }

private:
    Ret returnable_;
};

template<>
class returnable_action<void> : public action<void>
{
public:
    void invoke() { }
};

template<typename Ret, typename T>
class throwable_action : public action<Ret>
{
public:
    explicit throwable_action(T throwable)
        : throwable_(throwable)
    { }

    Ret invoke() { throw throwable_; }

private:
    T throwable_;
};

} // namespace ftrace

#endif

