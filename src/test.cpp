#include<bits/stdc++.h>
#include <glm/glm.hpp>

using namespace std;

namespace pr {
struct B {
    glm::vec3 x;
    glm::vec4 y;
};

struct A {
    vector<B> v;
    A() {
        v.push_back(B());
    }
    void x() {
        v.push_back(B());
        cout << v.size() << endl;
    }
};

}

using namespace pr;

int main() {
    A aa;
    aa.x();
}