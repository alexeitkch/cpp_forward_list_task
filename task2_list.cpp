//task2 list
#include <iostream>
//#include <assert.h>
#include <string>
#include <memory>

// struct Complex {
//     int re, im;
// };

// struct _Point {
//     int x{0}, y{0};
// };

class LinkedListIndexError : public std::exception {
    std::string msg_ex;
public:
    LinkedListIndexError(const char* msg) : msg_ex(msg) { }
    const char* what() const noexcept { return msg_ex.c_str(); }
};

template <typename T>
class Object {
    T data {0};
    std::shared_ptr<Object> next {nullptr};
    std::shared_ptr<Object> prev {nullptr};
public:
    Object(T dt) : data(dt), next(nullptr), prev(nullptr) { }
    T& get_data() { return data; }
    std::shared_ptr<Object>& get_next() { return next; }
    std::shared_ptr<Object>& get_prev() { return prev; }
};

template <typename T>
using shared_obj_ptr = std::shared_ptr<Object<T>>;

template <typename T>
class LinkedList {
    shared_obj_ptr<T> head {nullptr}; //указатель на первый объект в списке
    shared_obj_ptr<T> tail {nullptr}; //указатель на последний объект в списке
    
public:
    shared_obj_ptr<T>& get_head() { return head; }
    shared_obj_ptr<T>& get_tail() { return tail; }

    //в этой функции head указывает на первый созданный элемент списка, tail перенацеливается на новый объект
    void push_back(T data) {
        shared_obj_ptr<T> ptr_new_elm = std::make_shared<Object<T>>(data);
        // add first object
        if(head == nullptr && tail == nullptr) {
            head = ptr_new_elm; 
            tail = head; //head и tail указывают на единственный пока элемент списка
        } else { // add next objects
            tail->get_next() = ptr_new_elm; //заполнение поля next последнего элемента списка адресом нового объекта
            ptr_new_elm->get_prev() = tail; //заполнение поля prev нового элемента списка
            tail = ptr_new_elm; //перенацеливаем tail на новый элемент
        }
    } //..

    //в этой функции tail всегда указывает на первый созданный элемент списка head перенацеливается на новый объект
    void push_front(T data) {
        shared_obj_ptr<T> ptr_new_elm = std::make_shared<Object<T>>(data);
        // add first object
        if(head == nullptr && tail == nullptr) {
            head = ptr_new_elm;
            tail = head; //head и tail указывают на единственный пока элемент списка
        } else { // add next objects
            head->get_prev() = ptr_new_elm; //заполнение поля prev последнего элемента списка адресом нового объекта
            ptr_new_elm->get_next() = head; //заполнение поля next нового элемента списка
            head = ptr_new_elm; //перенацеливаем head на новый элемент
        }
    } //..
    
    void pop_back() {
        if(tail) {
            if(head == tail)
            {
                head = nullptr;
                tail = nullptr;
            } else {
                tail = tail->get_prev(); // tail перенацеливаем на адрес предыдущего элемента списка
                tail->get_next() = nullptr; // обнуляем указатель next  
            }
        }
    } //..
    
    void pop_front() {
        if(head) {
            if(head == tail)
            {
                head = nullptr;
                tail = nullptr;
            } else {
                head = head->get_next(); // tail перенацеливаем на адрес следущего элемента списка
                head->get_prev() = nullptr; // обнуляем указатель prev
            }
        }
    } //..

    void remove(int index) {
        if(index < 0) throw LinkedListIndexError("Invalid element index.");
        if(head == nullptr && index > 0) throw LinkedListIndexError("Invalid element index.");
        if(head)
        {
            if(index == 0) {
                pop_front();
            } else {
                shared_obj_ptr<T> ptr = head;
                shared_obj_ptr<T> ptr_next = nullptr;
                shared_obj_ptr<T> ptr_prev = nullptr;
                for(int i = 0; i < index; ++i) {
                    ptr = ptr->get_next();
                    if(ptr == nullptr && i < index)
                        throw LinkedListIndexError("Invalid element index.");
                }
                if(ptr == nullptr)
                {
                    pop_back();
                } else {
                    ptr_next = ptr->get_next();
                    ptr_prev = ptr->get_prev();
                    ptr_next->get_prev() = ptr_prev;
                    ptr_prev->get_next() = ptr_next;
                }
                ptr = nullptr;
            }
        }
    } //..

    void insert(int index, T data) {
        if(head == nullptr && index != 0) throw LinkedListIndexError("Invalid element index.");
        if(index < 0) throw LinkedListIndexError("Invalid element index.");
        if(index == 0)
            push_front(data);
        else {
            shared_obj_ptr<T> ptr = head;
            shared_obj_ptr<T> ptr_prev = nullptr;
            for(int i = 0; i < index; ++i) {
                ptr = ptr->get_next();
                if(ptr == nullptr && i < index-1)
                    throw LinkedListIndexError("Invalid element index.");
            }
            if(ptr == nullptr) push_back(data);
            else {
                ptr_prev = ptr->get_prev();
                shared_obj_ptr<T> ptr_new_elm = std::make_shared<Object<T>>(data);
                ptr_new_elm->get_next() = ptr;
                ptr_new_elm->get_prev() = ptr_prev;
                ptr->get_prev() = ptr_new_elm;
                ptr_prev->get_next() = ptr_new_elm;
            }
        }
    } //..

    T& operator[](int index) {
        if(head == nullptr) 
            throw LinkedListIndexError("Invalid element index.");
        shared_obj_ptr<T> ptr = head;
        //отрицательный индекс
        if(index < 0) throw LinkedListIndexError("Invalid element index.");
        //положительный индекс
        for(int i = 0; i < index; ++i) {
            ptr = ptr->get_next();
            if(ptr == nullptr && i < index-1)
                throw LinkedListIndexError("Invalid element index.");
        }
        return ptr->get_data();
    }
};

int main(void)
{
    LinkedList<std::string> lst; // создание пустого двусвязного списка

    lst.push_back("hello"); // добавление элемента в конец
    lst.push_back("C++");
    lst.push_front("OOP"); // добавление элемента в начало
    lst.push_front("Balakirev");
    lst.push_front("Sergey");
    lst.pop_back(); // удаление последнего элемента
    lst.pop_front(); // удаление первого элемента
    
    //Sergey Balakirev OOP hello C++
    lst.remove(0); // удаление элемента по индексу indx (индекс с нуля: 0, 1, ...)
    lst.insert(1, "Insert_elemenet"); // вставка элемента так, чтобы у него был индекс indx

    std::string s = lst[0]; // получение данных из элемента с индексом 0
    std::cout << s << std::endl;
    lst[0] = "hello"; // изменение данных в элементе с индексом 0

    LinkedList<unsigned> lst_u; // еще один двусвязный список

    lst_u.push_back(1);
    lst_u.push_back(2);
    lst_u.push_back(3);

    unsigned var = lst_u[0];
    std::cout << var << std::endl;
    lst_u[0] = 15;

    // перебор элементов с конца в начало
    std::shared_ptr< Object<std::string> > ptr_lst = lst.get_tail();
    while(ptr_lst) {
        std::string res = ptr_lst->get_data();
        std::cout << res << " ";
        ptr_lst = ptr_lst->get_prev();
    }
    std::cout << std::endl;

    // перебор элементов с начала и до конца
    std::shared_ptr< Object<unsigned> > ptr_lst_u = lst_u.get_head();
    while(ptr_lst_u) {
        unsigned a = ptr_lst_u->get_data();
        std::cout << a << " ";
        ptr_lst_u = ptr_lst_u->get_next();
    }
    std::cout << std::endl;

    //проверка индексов
    try {
        std::string s1 = lst[-1];
    }
    catch(const LinkedListIndexError& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        lst.insert(-2, "abc");
    }
    catch(const LinkedListIndexError& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        lst_u.remove(100);
    }
    catch(const LinkedListIndexError& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
