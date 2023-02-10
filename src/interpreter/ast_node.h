#pragma once

namespace zel {

namespace identifier {

class Node {

    enum Type {
        APDU = 0,
        STRING
    };


  public:
    Node();
    ~Node();

  private:
    Type type_;


};

} // namespace identifier

} // namespace zel