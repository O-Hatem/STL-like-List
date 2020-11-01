#include <iostream>
#include <algorithm>
#include <utility> // swap
#include <initializer_list>
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

struct must_be_neutral : public exception{
   const char* what() const throw(){
       return "The parameter of the function must be 0 or a positive number";
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
           using pointer = type*;
           using reference = type&;
           using iterator_category = std::bidirectional_iterator_tag;
       public:

           // Ctor
           iterator(node *temp = nullptr): value(temp){}

           // Shallow Copy Ctor
           // iterator(const iterator &rhs):value(){}

           // Deep Copy Ctor ( Problem(deep copy don't work): *(this->value) = *(rhs.value);)
           iterator(const iterator &rhs){
               cout << "Deep Copy" << endl;
               (this->value) = (rhs.value);
           }

           // copy-swap-idiom copy assignment operator
           // const-reference parameter will be the same as value parameter
           iterator& operator=(const iterator &rhs){
               if (this != &rhs){
                   cout << "copy-swap-idiom copy assignment operator\n";
                   iterator temp = rhs;

                   swap(value, temp.value); // Why this line call the copy ctor
               }
               return *this;
           }

           // move ctor
           iterator(iterator &&rhs){
               this->value = rhs.value;
               rhs.value = nullptr;
           }

           // move assignment operator
           iterator operator=(iterator&& rhs){
               if(this != &rhs){
                   delete value; // to free any resources (such as memory) from the object that is being assigned to.

                   value = rhs.value;

                   rhs.value = nullptr;
               }
               return *this;
           }


           // Pre increment
           iterator& operator++(){
//                if(this->value == last)
//                    throw after_last_exception();
               this->value = value->next;
               return (*this);
           }

           // Pre decrement
           iterator& operator--(){
//                if(this->value == List<type>::first)
//                    throw before_first_exception();
               value = value->prev;
               return(*this);
           }

           iterator operator++(type){iterator temp = *this;++(*this);return temp;}
           iterator operator--(type){iterator temp = *this; --(*this); return temp;}

           type& operator*(){return value->item;}

           bool operator==(const iterator &rhs) const {return (value == rhs.value);}
           bool operator!=(const iterator &rhs) const {return !(value == rhs.value);}

           bool operator<(const iterator &rhs) const {return (value < rhs.value);}
           bool operator>(const iterator &rhs) const {return !(value < rhs.value);}

           bool operator<=(const iterator &rhs) const {return (*this < rhs) || (this->value == rhs.value);}
           bool operator>=(const iterator &rhs) const {return (*this > rhs) || (this->value == rhs.value);}

           // returns a distance between two pointers
           int operator-(iterator rhs){
               // (*this - rhs) + 1
               iterator _this = *this;
               int counter = 0;

               if(_this > rhs){
                   while(_this >= rhs){
                       ++counter;
                       _this.value = _this.value->prev;
                   }
                   return counter+1;
               }else{
                   while(_this <= rhs){
                       --counter;
                       rhs.value = rhs.value->prev;
                   }
                   return counter-1;
               }
           }

           void operator+=(size_t n){
               if(n<0) return;
               while(n--){
                   ++(*this);
               }
           }

           void operator-=(size_t n){
               if(n<0) return;
               while(n--){
                   --(*this);
               }
           }

           iterator operator+(size_t n){
               iterator _this = *this;
               while(n--){
                   ++_this;
               }
               return _this;
           }

           iterator operator-(size_t n){
               iterator _this = *this;
               while(n--){
                   --_this;
               }
               return _this;
           }
   };

   iterator begin() const {return first;}
   iterator end() const {return last;}

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

   // initializer list Ctor
   List(initializer_list<type> il){
       node* root = new node();
       first = last = root;
       elements_counter = 0;

       for(auto x: il){
           this->insert(x,this->end());
       }
   }

   // copy constructor for list
   List<type>(const List<type> &another_list){
       node* root = new node();
       first = last = root;
       elements_counter = 0;

       for(auto x : another_list){
           this->insert(x, this->end());
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




   List<type>& operator= (List<type> another_list){
       this->delete_all();

       List<type>::iterator it = another_list.begin();
       while(it != another_list.end()){
           this->insert(it.value->item, this->end());
           ++it;
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

// Unary function to test count_if
bool isEven(int x){
   return (x%2==0);
}

// Unary function to test count_if
bool isOdd(int x){
   return !isEven(x);
}

// Binary function to test sort
bool isSmaller(int x, int y){
   return (x>y);
}


int main(){
   vector<int> v1 = {9,6,7};
   vector<int> v2 = {9,12,3};

   cout << (v1 <= v2) << endl;


   return 0;
}
