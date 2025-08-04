//task1 forward_list
#include <iostream>
#include <memory>

struct Complex {
    int re;
    int im;
};

class LinkedListIndexError : public std::exception {
    std::string msg_ex;
public:
    LinkedListIndexError(const char* msg) : msg_ex(msg) 
    { }
    const char* what() const noexcept { return msg_ex.c_str(); }
};

template <typename T>
class Object {
    T data {0};
    std::shared_ptr<Object> next {nullptr};
public:
    Object(T dt) : data(dt), next(nullptr) { }
    T& get_data() { return data; }
    std::shared_ptr<Object>& get_next() { return next; }
};

template <typename T>
using shared_obj_ptr = std::shared_ptr<Object<T>>;

template <typename T>
class OneLinkedList {
    shared_obj_ptr<T> head {nullptr}; //указатель на первый объект в списке
    shared_obj_ptr<T> tail {nullptr}; //указатель на последний объект в списке
    
public:
    shared_obj_ptr<T> get_head() { return head; }
    
    //в этой функции head всегда указывает на первый созданный элемент списка
    void push_back(T data)
    {
        shared_obj_ptr<T> ptr_new_elm = std::make_shared<Object<T>>(data);
        //добавление первого элемента в списк
        if(head == nullptr && tail == nullptr) 
        {
            ptr_new_elm->get_next() = head; //заполнение поля next элемента в этом случае там будет указатель nullptr
            head = ptr_new_elm; //head и tail указывают на один единственный пока элемент списка
            tail = head;
        } 
        else 
        { //добавление следующих элементов
            ptr_new_elm->get_next() = nullptr; //заполнение поля next - указатель nullptr т.к. он будет последний
            tail->get_next() = ptr_new_elm;
            tail = ptr_new_elm; //tail указывает на вставленный в конец элемент списка
        }
    }

    //в этой функции tail всегда указывает на первый созданный элемент списка
    void push_front(T data)
    {
        shared_obj_ptr<T> ptr_new_elm = std::make_shared<Object<T>>(data);
        //добавление первого элемента в списк
        if(head == nullptr && tail == nullptr) 
        {
            ptr_new_elm->get_next() = head; //заполнение поля next элемента в этом случае там будет указатель nullptr
            head = ptr_new_elm; //head и tail указывают на один единственный пока элемент списка
            tail = ptr_new_elm;
        } 
        else 
        {
            ptr_new_elm->get_next() = head;
            head = ptr_new_elm;
        }
    }
    
    void pop_back()
    {
        if(tail)
        {
            shared_obj_ptr<T> ptr {head};
            shared_obj_ptr<T> ptr_bef {nullptr}; //указатель на предыдущий элемент
            //проходим до конца список
            while(ptr) {
                if(ptr->get_next()) ptr_bef = ptr;
                ptr = ptr->get_next();
            }
            tail = ptr_bef;
            ptr_bef->get_next() = nullptr; 
        } 
    }
    
    void pop_front()
    {
        if(head)
        {
            shared_obj_ptr<T> ptr = head;
            head = head->get_next();
        }
    }

    T& operator[](int index)
    {
        if(head == nullptr) throw LinkedListIndexError("Invalid element index.");
        shared_obj_ptr<T> ptr = head;
        //отрицательный индекс
        if(index < 0) throw LinkedListIndexError("Invalid element index.");
        //положительный индекс
        for(int i = 0; i < index; ++i)
        {
            ptr = ptr->get_next();
            if(ptr == nullptr && i < index-1) //превышение индекса
            throw LinkedListIndexError("Invalid element index.");
        }
        return ptr->get_data();
    }
};

int main(void)
{
    //здесь продолжайте функцию main
    OneLinkedList<Complex> lst; // пустой односвязный список для хранения данных типа Complex (структура)

    lst.push_back(Complex {1, 2}); // добавление в конец списка
    lst.push_back(Complex {3, 4}); // добавление в конец списка
    lst.push_front(Complex {-1, -2}); // добавление в начало списка
    lst.push_front(Complex {-3, -4}); // добавление в начало списка

    lst.pop_back(); // удаление последнего элемента (если его нет, то ничего не делать)
    lst.pop_front(); // удаление первого элемента (если его нет, то ничего не делать)

    Complex d = lst[1];  // получение первого элемента по индексу
    std::cout << "[]Complex {" << d.re << ", " << d.im << "} : " << std::endl;
    lst[0] = Complex {5, 8}; // запись в первый элемент по индексу
    
    OneLinkedList<int> lst_int; // еще один односвязный список для хранения данных типа int 

    lst_int.push_back(1); // добавление в конец списка
    lst_int.push_back(2);
    lst_int.push_back(3);

    int var = lst_int[2]; // чтение данных из второго элемента списка
    std::cout << "[]int " << var << std::endl;
    lst_int[2] = -5; // запись данных в третий элемент списка

    // перебор первого списка
    std::shared_ptr< Object<Complex> > ptr_lst = lst.get_head();
    while(ptr_lst) {
        Complex res = ptr_lst->get_data();
        std::cout << "Complex {" << res.re << ", " <<res.im << "} : ";
        ptr_lst = ptr_lst->get_next();
    }
    std::cout << std::endl;

    // перебор второго списка
    std::shared_ptr< Object<int> > ptr_lst_int = lst_int.get_head();
    while(ptr_lst_int) {
        int a = ptr_lst_int->get_data();
        std::cout << "int " << a << " : ";
        ptr_lst_int = ptr_lst_int->get_next();
    }
    std::cout << std::endl;

    try {
        Complex cmp = lst[-1];
    } catch(const LinkedListIndexError& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
