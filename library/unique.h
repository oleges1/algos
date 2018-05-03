#include <cstdlib>
#include <algorithm>
#include <memory>

template <typename T, class Deleter = std::default_delete<T>>
class UniquePtr {
 private:
    std::tuple< T *, Deleter> data;

 public:
    explicit UniquePtr(T * src = nullptr) {
        data = std::make_tuple(src, Deleter());
    }

    UniquePtr(T * src, Deleter deleter): data(src, deleter) {
    }

    UniquePtr(const UniquePtr&) = delete;  // запрещаем копирование и присваивание
    void operator = (const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& tmp) {
        std::get<1>(data) = Deleter();
        std::get<0>(data) = std::get<0>(tmp.data);
        std::get<0>(tmp.data) = nullptr;  // временный объект всё равно вот-вот умрёт,
            // так что запишем в его ptr значение nullptr, чтобы его деструктор не удалил данные
    }

    UniquePtr& operator = (UniquePtr&& tmp) {  // аналогичный оператор присваивания для временн
        std::get<1>(data)(std::get<0>(data));  // старые данные надо сначала удалить,
        std::get<0>(data) = std::get<0>(tmp.data);
        std::get<1>(data) = std::get<1>(tmp.data);  // а потом надо забрать владение новыми данными
        std::get<0>(tmp.data) = nullptr;  // а сам временный объект скоро всё равно будет умирать
        return *this;
    }

    UniquePtr& operator = (T * src) {  // умному указателю можно переприсвоить и обычный указатель
        std::get<1>(data)(std::get<0>(data));
        std::get<0>(data) = src;
        return *this;
    }

    ~UniquePtr() {
        std::get<1>(data)(std::get<0>(data));
    }

    Deleter& get_deleter() noexcept {
        return std::get<1>(data);
    }

    const Deleter& get_deleter() const noexcept {
        return std::get<1>(data);
    }

    T& operator * () noexcept {
        return *std::get<0>(data);
    }

    const T& operator * () const noexcept {
        return *std::get<0>(data);
    }

    T * operator -> () const noexcept {
        return std::get<0>(data);
    }

    T * release() {
        auto temp = std::get<0>(data);
        std::get<0>(data) = nullptr;
        return temp;
    }

    void reset(T * src) {
        std::get<1>(data)(std::get<0>(data));
        std::get<0>(data) = src;
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(std::get<0>(data), std::get<0>(other.data));
    }

    T * get() const noexcept {
        return std::get<0>(data);
    }

    explicit operator bool() const noexcept {
        return (std::get<0>(data) != nullptr);
    }
};
