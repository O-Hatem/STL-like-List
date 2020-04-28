#include <iostream>
#include <exception>
using namespace std;

struct after_last_exception : public exception{
    const char* what() const throw(){
        return "You Passed the end";
    }
};

struct before_first_exception : public exception{
    const char* what() const throw(){
        return "You Passed the beginning";
    }
};

template <class type>
class List {
private:
    // Node
    struct node{
        type item;
        node *next;
        node *prev;
    };

    node* first;
    node* last; // first node after the last element
    int elements_counter;

public:

    struct iterator{
            node *value;

            using difference_type = type;
            using value_type = type;
            using pointer = const type*;
            using reference = const type&;
            using iterator_category = std::bidirectional_iterator_tag;
        public:

            // Ctor
            iterator(node *temp = nullptr): value(temp){}

            // Post increment
            iterator operator++(){
//                if(this->value == last)
//                    throw after_last_exception();
                value = value->next;
                return (*this);
            }

            // Pre increment
            iterator operator++(type){
                iterator temp = *this;
                ++(*this);
                return temp;
            }

            // Post decrement
            iterator operator--(){
//                if(this->value == List<type>::first)
//                    throw before_first_exception();
                value = value->prev;
                return(*this);
            }

            // Pre decrement
            iterator operator--(type){iterator temp = *this; --(*this); return temp;}

            type& operator*(){return value->item;}

            bool operator==(iterator rhs){return (value == rhs.value);}
            bool operator!=(iterator rhs){return !(value == rhs.value);}

            bool operator<(iterator rhs){return (value < rhs.value);}
            bool operator>(iterator rhs){return !(*this < rhs);}

            void operator+=(int n){
                if(n<0) return;
                while(n--){
                    ++(*this);
                }
            }

            void operator-=(int n){
                if(n<0) return;
                while(n--){
                    --(*this);
                }
            }
    };

    iterator begin() {return first;}
    iterator end() {return last;}

    int size(){return elements_counter;}
    bool empty(){return !(elements_counter);}

    // Default Ctor
    List(){
        node* root = new node();
        // root->item is garbage
        first = last = root;
        elements_counter = 0;
    }

    //Parameterized Ctor
    List(type value , int initial_size){
        node* root = new node();
        first = last = root;
        elements_counter = 0;

        while(initial_size--){
            insert(value, List::end());
        }
    }

    // Dtor
    ~List(){
        delete_all();
    }

    void insert(type value, iterator position){
        node* newnode = new node();
        newnode->item = value;

        if(position == begin()){
            // Insert at the beginning
            //cout << "INSERT BEGINNING\n";
            newnode->next = first;
            newnode->prev = nullptr;
            first->prev = newnode;
            first = newnode;
        }else if(position == end()){
            // Insert at the end
            //cout << "INSERT END\n";
            newnode->next = nullptr;
            newnode->prev = last;
            last->next = newnode;
            last->item = newnode->item;
            last = newnode;

        }else{
            //cout << "INSERT MIDDLE\n";
            node *pos = position.value;
            newnode->next = pos;
            newnode->prev = pos->prev;
            pos->prev->next = newnode;
            pos->prev = newnode;
        }
        elements_counter++;
    }

    iterator erase(iterator position){

        if(position > end())
            throw after_last_exception();

        if(position == begin()){
            node *temp = first;
            first = first->next;
            delete temp;
            temp = nullptr;

            elements_counter--;
            return first;
        }else if(position == end()){
            node *temp = last;
            last = last->prev;
            last->next = nullptr;

            delete temp;
            temp = nullptr;

            elements_counter--;
            return last;
        }else{
            node *pos = position.value;
            node *next = pos->next;
            pos->prev->next = pos->next;
            pos->next->prev = pos->prev;

            delete pos;
            pos = nullptr;

            elements_counter--;
            return next;
        }
    }

    List<int>& operator= (List<type> &another_list){
        List<type>::iterator current = another_list.begin();
        while(current != another_list.end()){
            this->insert(current.value->item, this->end());
            current = current.value->next;
        }
        return *this;
    }

    void printList(){
        node* temp = first;
        cout << "List elements: [ ";
        while(temp != last){
            cout << temp->item << ' ';
            temp = temp->next;
        }
        cout << "]" << endl;
    }

private:
    // Delete all
    void delete_all(){
        // Deletes from the beginning to end
        while(first != last){
            node* temp;
            temp = first;
            first = first->next;

            free(temp);
            temp = nullptr;
            elements_counter--;
        }
    }

};

int main()
{
    int x = 1;
    try{
        List<int> l;

        l.insert(x++, l.end()); // 1
        l.insert(x++, l.end()); // 1 2
        l.insert(x++, l.end()); // 1 2 3
        l.insert(x++, l.end()); // 1 2 3 4
        l.insert(x++, l.begin()); // 5 1 2 3 4

        List<int>::iterator it = l.begin();
        it+=3; // it => 3
        l.insert(x++,it); // 5 1 2 6 3 4

        it--; // it => 6
        cout << "Iterator value: " << *it << endl;

        List<int>::iterator it2 = l.end();
        it2-=3; // it2 => 6
        cout << "Iterators Equal: " << boolalpha << (it == it2) << endl;

        List<int> temp(4,3);
        temp.printList();

        temp = l;
        temp.printList();

        it = temp.begin();
        it = temp.erase(it);
        it = temp.erase(it);
        it++;
        it = temp.erase(it);
        temp.printList();

        cout << "\nL: ";
        l.printList();
        cout << "size: " << l.size() << endl;
        cout << "Is Empty: " << boolalpha << l.empty() << endl;
    }catch(exception e){
        cout << e.what() << endl;
    }
    return 0;
}
