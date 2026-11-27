#include <iostream>

class Foo {
    private:
        int att;

    public:
        Foo(int attr)
            : att(attr) {}

        void display() {
            std::cout << this->att << std::endl;
        }

        void add() {
            this->att++;
        }
};

int main() {
    Foo obj = Foo(15);
    obj.display();
    obj.add();
    obj.display();

    return 0;
}