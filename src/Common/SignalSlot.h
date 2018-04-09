#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <functional>
#include <map>

template <typename... Args>
class Signal
{
public:
    Signal() : m_current_id(0) {}
    Signal(Signal const& other) : m_current_id(0) {} // Конструктор копирования нового сигнала

    // Соединение функции-члена класса с заданным сигналом
    template <typename T>
    int connect_member(T *inst, void (T::*func)(Args...))
    {
        return connect([=](Args... args)
        {
            (inst->*func)(args...);
        });
    }

    // Соединение константной функции-члена класса с заданным сигналом
    template <typename T>
    int connect_member(T *inst, void (T::*func)(Args...) const)
    {
        return connect([=](Args... args) {
            (inst->*func)(args...);
        });
    }

    // Соединяет std::function с сигналом. Возвращаемое значение
    // можно использовать для отключения соединения
    int connect(std::function<void(Args...)> const& slot) const
    {
        m_slots.insert(std::make_pair(++m_current_id, slot));
        return m_current_id;
    }

    // Отключение подключенной функции
    void disconnect(int id) const { m_slots.erase(id); }
    // Отключение всех подключенных функций
    void disconnect_all() const { m_slots.clear(); }
    // Присваивание создает новый сигнал
    Signal& operator=(Signal const& /*other*/) { disconnect_all(); }

    // Вызов всех подключенных функций
    void Emit(Args... p)
    {
        for(auto it : m_slots)
        {
            it.second(p...);
        }
    }

private:
    mutable std::map<int, std::function<void(Args...)>> m_slots;
    mutable int m_current_id;
};

#endif // SIGNAL_HPP
